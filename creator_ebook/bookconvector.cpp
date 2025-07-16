#include "bookconvector.h"
#include <QFileInfo>
#include <QDebug>
#include <algorithm>
#include <QRegularExpression>


// #include <cld3/nnet_language_identifier.h>

struct SentencePart {
    int ordinal;
    QString text;
    QString lang;
};

QVector<SentencePart> splitSentence(const QString &sentence) {
    QVector<SentencePart> result;
    int ordinal = 1;

    QRegularExpression re(R"((«[^»]+»|\"[^\"]+\"|\([^)]+\)|\[[^\]]+\]|[a-zA-Zа-яА-ЯёЁ]+|[\u4e00-\u9fff]+|[^\s]+))");
    auto it = re.globalMatch(sentence);
    while (it.hasNext()) {
        auto match = it.next();
        QString chunk = match.captured(0).trimmed();
        if (chunk.isEmpty()) continue;
        result.append({ordinal++, chunk, ""});
    }
    return result;
}

// QString detectLangCLD3(const QString& text) {
//     // Один идентификатор на поток (дешёвый!)
//     // max_num_bytes, min_num_bytes: посмотрите readme CLD3, эти значения дефолтны для большинства задач
//     static chrome_lang_id::NNetLanguageIdentifier identifier(0, 1000);
//     auto result = identifier.FindLanguage(text.toStdString());
//     return QString::fromStdString(result.language);
// }

// QVector<SentencePart> processSentence(const QString &sentence) {
//     QVector<SentencePart> parts = splitSentence(sentence);
//     for (auto& part : parts) {
//         part.lang = detectLangCLD3(part.text);
//     }
//     return parts;
// }



BookConvector::BookConvector(QObject *parent)
    : QObject{parent}
{
    _pfdReader = new PdfReader();
    _ocrReader = new OcrReader();
    connect(_pfdReader, &PdfReader::alert, this, &BookConvector::alert);
    connect(_ocrReader, &OcrReader::alert, this, &BookConvector::alert);
}

BookConvector::~BookConvector()
{
    _pfdReader->closeDocument();
    delete _pfdReader;
}



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
            qDebug() << s << "\n";


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







