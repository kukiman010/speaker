#ifndef BOOKCONVECTOR_H
#define BOOKCONVECTOR_H

#include <QObject>

#include "pdfreader.h"
#include "ocrreader.h"
#include "languagedetector.h"
#include "book.h"


class BookConvector : public QObject
{
    Q_OBJECT

public:
    explicit BookConvector(QObject *parent = nullptr);
    ~BookConvector();

    QStringList splitTextSentences(const QString& text);
    // QVector<SentencePart> processSentence(const QString &sentence);
    // QVector<SentencePart> splitSentence(const QString &sentence);
    QVector<SentencePart> segmentTextByLanguage(const QString& input, int ngram = 5);


public slots:
    void conver(QString filePath);

signals:
    void alert(const QString & title, const QString & text, QString type);

private:
    PdfReader *                 _pfdReader;
    OcrReader *                 _ocrReader;
    FastTextLanguageDetector *  _langDetect;
};

#endif // BOOKCONVECTOR_H
