#ifndef OCRREADER_H
#define OCRREADER_H

#include <QObject>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


class OcrReader : public QObject
{
    Q_OBJECT
public:
    OcrReader(QObject* parent = nullptr);

    QString                 runTesseract(const QString& imagePath, const QString& lang = "rus");

signals:
    void                    alert(const QString & title, const QString & text, QString type);
};

#endif // OCRREADER_H
