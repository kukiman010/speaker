#include "pdfreader.h"

#include <QDebug>

PdfReader::PdfReader(QObject* parent)
    : QObject(parent), _ctx(nullptr), _doc(nullptr), m_pageCount(0), m_textPages(0), m_imageOnlyPages(0)
{
    _ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (!_ctx)
    {
        emit alert("PdfReader::PdfReader", tr("Ошибка создания контекста MuPDF!"), "error");
        return;
    }
    fz_register_document_handlers(_ctx);
}

PdfReader::~PdfReader()
{
    closeDocument();
    if (_ctx) {
        fz_drop_context(_ctx);
        _ctx = nullptr;
    }
}

bool PdfReader::openDocument(const QString& fileWay)
{
    closeDocument();

    filePath = fileWay;
    QByteArray fileNameBytes = fileWay.toUtf8();
    const char* fileName = fileNameBytes.constData();

    fz_try(_ctx)
    {
        _doc = fz_open_document(_ctx, fileName);
    }
    fz_catch(_ctx)
    {
        emit alert("PdfReader::openDocument", tr("Не удалось открыть PDF документ"), "error");
        _doc = nullptr;
        return false;
    }

    analyzePages();
    return true;
}

void PdfReader::closeDocument()
{
    if (_doc)
    {
        fz_drop_document(_ctx, _doc);
        _doc = nullptr;
    }
}

QString PdfReader::extractTextFromPage(int pageNumber)
{
    if (!_doc || pageNumber < 0 || pageNumber >= m_pageCount)
        return QString();

    return extractOrderedTextLines(pageNumber);
}

LiteBook PdfReader::extractTextAllPages()
{
    LiteBook book;
    // QList<QString> texts;
    if (!_doc)
        return book;

    for (int i = 0; i < m_pageCount; ++i)
        book.book.insert(i, extractOrderedTextLines(i));

    book.pageCount = m_pageCount;
    // book.bookName = _doc->

    return book;
}

QList<int> PdfReader::findCorruptedPages()
{
    QList<int> corruptedPages;

    for (int i = 0; i < m_pageCount; ++i)
    {
        fz_page *page = fz_load_page(_ctx, _doc, i);
        fz_stext_page *textPage = fz_new_stext_page_from_page(_ctx, page, nullptr);

        bool isCorrupted = false;

        if (textPage) {
            for (fz_stext_block *block = textPage->first_block; block; block = block->next) {
                if (block->type == FZ_STEXT_BLOCK_TEXT) {
                    for (fz_stext_line *line = block->u.t.first_line; line; line = line->next) {
                        QString lineText;
                        for (fz_stext_char *ch = line->first_char; ch; ch = ch->next) {
                            lineText += QChar(ch->c);
                            qDebug() << "symbol:" << /*QChar(*/(char)ch->c/*)*/ << "| code:" << (int)ch->c;
                        }
                    }
                }
            }

            fz_drop_stext_page(_ctx, textPage);
        }
        fz_drop_page(_ctx, page);

        if (isCorrupted)
            corruptedPages.append(i);
    }

    return corruptedPages;
}

QString PdfReader::savePageAsImage(int pageNumber, const QString& outPathPrefix)
{
    if (!_doc || pageNumber < 0 || pageNumber >= m_pageCount)
        return QString();

    fz_page *page = fz_load_page(_ctx, _doc, pageNumber);

    // 1. Получаем габариты страницы (fz_rect)
    fz_rect bounds = fz_bound_page(_ctx, page);

    // 2. Округляем границы в fz_irect
    fz_irect bbox = fz_round_rect(bounds);

    // 3. Готовим цветовую схему и создаём картинку (fz_pixmap)
    fz_pixmap *pix = fz_new_pixmap_with_bbox(_ctx, fz_device_rgb(_ctx), bbox, nullptr, 1);

    fz_matrix ctm = fz_identity;
    fz_device *dev = fz_new_draw_device(_ctx, ctm, pix);

    // 4. Рендерим страницу
    fz_run_page(_ctx, page, dev, ctm, nullptr);

    fz_close_device(_ctx, dev);
    fz_drop_device(_ctx, dev);

    // 5. Сохраняем картинку в файл
    QString fileName = QString("%1_page_%2.png").arg(outPathPrefix).arg(pageNumber);
    QByteArray fileNameBytes = fileName.toUtf8();

    fz_save_pixmap_as_png(_ctx, pix, fileNameBytes.constData());

    // Очистка ресурсов
    fz_drop_pixmap(_ctx, pix);
    fz_drop_page(_ctx, page);

    return fileName;
}

bool PdfReader::compareLines(const LineItem &a, const LineItem &b)
{
    if (fabs(a.y0 - b.y0) > 2.0)
        return a.y0 < b.y0;
    return a.x0 < b.x0;
}

std::string PdfReader::utf32_to_utf8(int codepoint)
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

QString PdfReader::extractOrderedTextLines(int pageNumber)
{
    fz_page *page = fz_load_page(_ctx, _doc, pageNumber);
    fz_stext_page *textPage = fz_new_stext_page_from_page(_ctx, page, nullptr);

    QString allText;

    if (textPage)
    {
        for (fz_stext_block *block = textPage->first_block; block; block = block->next)
        {
            if (block->type == FZ_STEXT_BLOCK_TEXT)
            {
                for (fz_stext_line *line = block->u.t.first_line; line; line = line->next)
                {
                    for (fz_stext_char *ch = line->first_char; ch; ch = ch->next)
                    {
                        allText.append(QChar(ch->c));
                    }
                    allText.append('\n');
                }
            }
        }
        fz_drop_stext_page(_ctx, textPage);
    }
    fz_drop_page(_ctx, page);

    return allText;
}

void PdfReader::analyzePages()
{
    m_pageCount = fz_count_pages(_ctx, _doc);
    m_textPages = 0;
    m_imageOnlyPages = 0;

    for (int i = 0; i < m_pageCount; ++i)
    {
        fz_page *page = fz_load_page(_ctx, _doc, i);

        fz_stext_page *textPage = fz_new_stext_page_from_page(_ctx, page, nullptr);
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
                        {
                            hasText = true;
                            break;
                        }
                    }
                }
                if (hasText) break;
            }
            fz_drop_stext_page(_ctx, textPage);
        }

        if (hasText)
            ++m_textPages;
        else
            ++m_imageOnlyPages;

        fz_drop_page(_ctx, page);
    }
    qDebug() << "Страниц:" << m_pageCount
             << "Текстовых страниц:" << m_textPages
             << "Страниц только с картинками:" << m_imageOnlyPages;
}
