#ifndef BOOKCONVECTOR_H
#define BOOKCONVECTOR_H

#include <QObject>
#include "pdfreader.h"
#include "ocrreader.h"


class BookConvector : public QObject
{
    Q_OBJECT

    PdfReader *         _pfdReader;
    OcrReader *         _ocrReader;
public:


    explicit BookConvector(QObject *parent = nullptr);
    ~BookConvector();

    QStringList splitTextSentences(const QString& text);




public slots:
    void conver(QString filePath);

signals:
    void alert(const QString & title, const QString & text, QString type);

};

#endif // BOOKCONVECTOR_H
