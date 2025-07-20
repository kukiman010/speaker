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

    #ifdef Q_OS_WIN
        _langDetect = new FastTextLanguageDetector("D:/users/qt_projects/speaker/speaker/libs/fastText/data/lid.176.bin");
    #elif defined(Q_OS_MAC)
        qDebug() << "Это macOS";
    #elif defined(Q_OS_LINUX)
         _langDetect = new FastTextLanguageDetector("/tmp/lid.176.bin");
    #else
        qDebug() << "Другая ОС";
    #endif


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

        // QString testText = tr("Я назвал свои коллекции «grapefruit» (англ.: грейпфрут) и «lemon» (англ.: лимон)?");
        // QVector<SentencePart> parts2 = segmentTextByLanguage(testText);

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

QVector<SentencePart> BookConvector::atomicSegmentTextByLanguage(const QString &input)
{
    QVector<SentencePart> result;
    QRegularExpression tokenRegex(R"(\w+|[^\w\s]+|\s+)"); // слова, знаки, пробелы
    QRegularExpressionMatchIterator i = tokenRegex.globalMatch(input);

    QString currentLang;
    QString currentFragment;

    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString token = match.captured(0);

        // Пропускаем только пустые пробелы, если нужно сохранение их — уберите эту проверку
        if (token.trimmed().isEmpty())
        {
            currentFragment += token;
            continue;
        }

        QString lang = _langDetect->detectLanguage(token);
        if (lang.isEmpty())
            lang = currentLang;

        // Смена языка — новый фрагмент
        if (lang != currentLang && !currentFragment.isEmpty())
        {
            result.append({/*ordinal позже*/ 0, currentFragment, currentLang});
            currentFragment.clear();
        }
        currentLang = lang;
        currentFragment += token;
    }
    if (!currentFragment.isEmpty()) {
        result.append({0, currentFragment, currentLang});
    }
    return result;
}

QVector<SentencePart> BookConvector::mergeConsecutiveSameLang(const QVector<SentencePart> &atomicParts, int ngram)
{
    QVector<SentencePart> merged;
    int ordinal = 1;

    for (const SentencePart& part : atomicParts)
    {
        QString correctedLang = part.langCode;
        // if (maybeIsPopularLanguage(part.text))
            // correctedLang = detectPopularLanguage(part.text);

        if (!merged.isEmpty() && part.text.trimmed().length() < ngram)
        {
            merged.last().text += part.text;
            // merged.last().langCode — не меняем, short-words just attach!
            continue;
        }

        if (!merged.isEmpty() && merged.last().langCode == correctedLang)
            merged.last().text += part.text;
        else
            merged.append({ordinal++, part.text, correctedLang});
    }
    return merged;
}

QVector<SentencePart> BookConvector::segmentTextByLanguage(const QString &input)
{
    QVector<SentencePart> atoms = atomicSegmentTextByLanguage(input);
    return mergeConsecutiveSameLang(atoms);
}






