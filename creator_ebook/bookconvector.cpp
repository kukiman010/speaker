#include "bookconvector.h"
#include <QFileInfo>
#include <QDebug>
#include <algorithm>
#include <QRegularExpression>


BookConvector::BookConvector(QObject *parent)
    : QObject{parent}
{
    _pfdReader = new PdfReader();
    _ocrReader = new OcrReader();
    _langDetect = new FastTextLanguageDetector("/tmp/lid.176.bin");

    connect(_pfdReader, &PdfReader::alert, this, &BookConvector::alert);
    connect(_ocrReader, &OcrReader::alert, this, &BookConvector::alert);
}

BookConvector::~BookConvector()
{
    _pfdReader->closeDocument();
    delete _pfdReader;
}

// QVector<SentencePart> BookConvector::splitSentence(const QString &sentence)
// {
//     QVector<SentencePart> result;
//     int ordinal = 1;

//     QRegularExpression re(R"((«[^»]+»|\"[^\"]+\"|\([^)]+\)|\[[^\]]+\]|[a-zA-Zа-яА-ЯёЁ]+|[\u4e00-\u9fff]+|[^\s]+))");
//     auto it = re.globalMatch(sentence);
//     while (it.hasNext()) {
//         auto match = it.next();
//         QString chunk = match.captured(0).trimmed();
//         if (chunk.isEmpty()) continue;
//         result.append({ordinal++, chunk, ""});
//     }
//     return result;
// }

// QVector<SentencePart> BookConvector::processSentence(const QString &sentence)
// {
//     QVector<SentencePart> parts = splitSentence(sentence);
//     for (auto& part : parts) {
//         part.lang = _langDetect->detectLanguage(part.text);
//     }
//     return parts;
// }

void BookConvector::conver(QString filePath)
{
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix();

    if( extension == "pdf")
    {
        LiteBook book;
        if (_pfdReader->openDocument(filePath))
        {
            // qDebug() << "Всего страниц:" << _pfdReader->pageCount();
            // qDebug() << "Текст (первая страница):" << _pfdReader->extractTextFromPage(0);
            book = _pfdReader->extractTextAllPages();
        }
        // qDebug();

        QList<int> corrupted = _pfdReader->findCorruptedPages();
        // corrupted.append(477);
        if( !corrupted.isEmpty())
            emit alert(tr("BookConvector::conver"), tr("Найдены ошибки при извлечении текста ") + QString::number(corrupted.size()), "warning");
        // foreach(int i, corrupted)
        // {
            // QString imgPath = _pfdReader->savePageAsImage(i, "/tmp/pdf_image");
            // QString ocrText = _ocrReader->runTesseract(imgPath, "rus");
            // qDebug() << "OCR для страницы" << i << ":\n" << ocrText;
        // }

        // for(int i =0; i< book.book.size(); ++i)
        // {
            // qDebug() << book.book[i];
        //     QString ss = _pfdReader->extractTextFromPage(i);
        //     qDebug() << ss;

        // }
        // 475 + 5
        // QString ss = _pfdReader->extractTextFromPage(477);
        // QString ss = book.book[477];
        // qDebug() << ss;

        QString ss1 = _pfdReader->extractTextFromPage(254);
        qDebug() << ss1 << "\n\n\n";


        QStringList qsl = splitTextSentences(ss1);

        for(QString s: qsl)
        {
            // qDebug() << s << "\n";

            QVector<SentencePart> parts = segmentTextByLanguage(s);

            for (const SentencePart& part : parts)
            {
                qDebug() << part.text << "->" << part.langCode;
            }


        }

        // QString countryCode = _langDetect->detectLanguage("Я назвал свои коллекции «grapefruit» (англ.: грейпфрут) и «lemon» (англ.: лимон)?");



        qDebug();
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

QVector<SentencePart> BookConvector::segmentTextByLanguage(const QString &input, int ngram)
{
    QVector<SentencePart> result;
    QStringList words = input.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    QString currentLang;
    QString currentFragment;

    for (int i = 0; i < words.size(); )
    {
        QStringList ngramWords;
        int actualN = qMin(ngram, words.size() - i);
        for (int j = 0; j < actualN; ++j) {
            ngramWords << words[i + j];
        }
        QString fragment = ngramWords.join(' ');
        QString lang = _langDetect->detectLanguage(fragment);

        if (lang.isEmpty()) {
            lang = currentLang; // Можно также выбросить фрагмент, если язык не определён
        }

        if (currentLang != lang && !currentFragment.isEmpty()) {
            result.append({i, currentFragment.trimmed(), currentLang});
            currentFragment.clear();
        }

        currentLang = lang;
        currentFragment += fragment + " ";
        i += actualN;
    }
    if (!currentFragment.isEmpty() && !currentLang.isEmpty()) {
        result.append({1, currentFragment.trimmed(), currentLang});
    }
    return result;
}







