#include "bookconvector.h"
#include <QFileInfo>
#include <QDebug>
#include <algorithm>
#include <QRegularExpression>

#include <vector>
#include <string>
#include <sstream>


BookConvector::BookConvector(QObject *parent)
    : QObject{parent}
{}

bool BookConvector::compareLines(const LineItem &a, const LineItem &b)
{
    if (fabs(a.y0 - b.y0) > 2.0)
        return a.y0 < b.y0;
    return a.x0 < b.x0;
}

std::string BookConvector::utf32_to_utf8(int codepoint)
{
    std::string out;
    // skip invalid unicode
    if ((codepoint >= 0xD800 && codepoint <= 0xDFFF) || (codepoint < 0x20 && codepoint != 0x09 && codepoint != 0x0D && codepoint != 0x0A)) return out;
    if (codepoint <= 0x7F)
    {
        out += static_cast<char>(codepoint);
    }
    else if (codepoint <= 0x7FF)
    {
        out += static_cast<char>(0xC0 | (codepoint >> 6));
        out += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    else if (codepoint <= 0xFFFF)
    {
        out += static_cast<char>(0xE0 | (codepoint >> 12));
        out += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    else
    {
        out += static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07));
        out += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    return out;
}


std::string BookConvector::extractOrderedTextLines(fz_context *ctx, fz_document *doc, int pageNum)
{
    fz_page *page = fz_load_page(ctx, doc, pageNum);
    fz_stext_page *textPage = fz_new_stext_page_from_page(ctx, page, nullptr);

    struct LineItem { QString text; float x0, y0; };
    std::vector<LineItem> lines;

    for (fz_stext_block *block = textPage->first_block; block; block = block->next)
    {
        if (block->type != FZ_STEXT_BLOCK_TEXT) continue;
        for (fz_stext_line *line = block->u.t.first_line; line; line = line->next)
        {
            QString qtext;
            for (fz_stext_char *ch = line->first_char; ch; ch = ch->next)
            {
                if ((ch->c < 0x20 && ch->c != 0x09 && ch->c != 0x0A && ch->c != 0x0D)
                    || ch->c == 0xFFFD
                    || (ch->c >= 0xD800 && ch->c <= 0xDFFF))
                    continue; // игнорируем мусор/невидимые/битые символы

                qtext.append(QChar(ch->c));
            }
            lines.push_back({qtext, line->bbox.x0, line->bbox.y0});
        }
    }

    fz_drop_stext_page(ctx, textPage);
    fz_drop_page(ctx, page);

    std::sort(lines.begin(), lines.end(), [](const LineItem &a, const LineItem &b) {
        if (fabs(a.y0 - b.y0) > 2.0)
            return a.y0 < b.y0;
        return a.x0 < b.x0;
    });

    QString finalText;
    for (const auto &li : lines)
        finalText += li.text + QLatin1Char(' ');

    // Самая быстрая конвертация результата в std::string (UTF-8):
    return finalText.toUtf8().toStdString();
}


void BookConvector::conver(QString filePath)
{
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix();

    if( extension == "pdf")
    {

        fz_context *ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
        fz_register_document_handlers(ctx);
        if (!ctx)
        {
            qDebug()<< "Ошибка создания контекста MuPDF!\n";
            return;
        }

        QByteArray fileNameBytes = filePath.toUtf8();
        const char* fileName = fileNameBytes.constData();


        fz_document *doc = nullptr;
        fz_try(ctx)
        {
            doc = fz_open_document(ctx, fileName);
        }
        fz_catch(ctx)
        {
            qDebug()<< "Не удалось открыть PDF документ!\n";
            fz_drop_context(ctx);
            return;
        }

        int pageCount = fz_count_pages(ctx, doc);
        int imageOnlyPages = 0;
        int textPages = 0;

        for (int i = 0; i < pageCount; ++i) {
            fz_page *page = fz_load_page(ctx, doc, i);

            // Получить извлечённый текст (структурированная страница)
            fz_stext_page *textPage = fz_new_stext_page_from_page(ctx, page, nullptr);

            bool hasText = false;
            if (textPage)
            {
                for (fz_stext_block *block = textPage->first_block; block; block = block->next)
                {
                    if (block->type == FZ_STEXT_BLOCK_TEXT)
                    {
                        for (fz_stext_line *line = block->u.t.first_line; line; line = line->next)
                        {
                            if (line->first_char)
                            { // если есть хотя бы один символ в строке
                                hasText = true;
                                break;
                            }
                        }
                    }
                    if (hasText) break;
                }
                fz_drop_stext_page(ctx, textPage);
            }


            if (hasText)
                textPages++;
            else
                imageOnlyPages++;

            fz_drop_page(ctx, page);
        }

        qDebug() << "Страниц: " << pageCount;
        qDebug() << "Текстовых страниц: " << textPages;
        qDebug() << "Страниц только с картинками: " << imageOnlyPages;

        std::list<std::string> book;

        if (imageOnlyPages > textPages)
        {
            qDebug()<< "PDF по большей части состоит из фотографий! Ошибка разбора текста.\n";
        }
        else if (textPages == 0)
        {
            qDebug()<< "Нет текстовых страниц.\n";
        }
        else
        {
            qDebug() << "PDF содержит текст! Разбиваем на текстовые блоки на первой странице:\n";

            for( int i=0; i< pageCount; ++i)
            {
                std::string orderedText = extractOrderedTextLines(ctx, doc, i);
                // qDebug() << QString::fromStdString(orderedText);
                book.push_back( orderedText );
            }
        }

        fz_drop_document(ctx, doc);
        fz_drop_context(ctx);

        for( const std::string & s : book)
            qDebug() << QString::fromStdString(s);


    }
    else
        emit alert(tr("Ошибка формата файла"), tr("в данный момент программа не умеет работать с ") + extension, "warning");
}


QStringList BookConvector::splitTextSentences(const QString& text)
{
    static const int maxMessageLength = 980;
    static const int hardBreakPoint = 650;
    QRegularExpression sentenceEndings(R"((?<=[.!?])\s)");

    QStringList sentences = text.split(sentenceEndings, Qt::SkipEmptyParts);
    QStringList result;

    for (QString sentence : sentences)
    {
        sentence = sentence.trimmed();
        if (sentence.isEmpty()) continue;

        if (sentence.length() > maxMessageLength)
        {
            int start = 0;
            while (start < sentence.length())
            {
                // int end = std::min(start + maxMessageLength, sentence.length());
                int end = static_cast<int>(std::min<qsizetype>(
                    static_cast<qsizetype>(start + maxMessageLength),
                    sentence.length()
                    ));

                int spacePos = sentence.lastIndexOf(' ', end - 1);
                if (spacePos < start)
                    spacePos = end;
                result.append(sentence.mid(start, spacePos - start));
                start = spacePos + 1;
            }
        } else if (sentence.length() > hardBreakPoint)
        {
            int newlinePos = sentence.indexOf('\n');
            if (newlinePos != -1 && newlinePos < hardBreakPoint)
            {
                result.append(sentence.left(newlinePos));
                QString remainder = sentence.mid(newlinePos + 1);
                if (!remainder.isEmpty())
                    result.append(remainder);
            }
            else
            {
                result.append(sentence);
            }
        } else {
            result.append(sentence);
        }
    }
    return result;
}
