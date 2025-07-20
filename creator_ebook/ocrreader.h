#ifndef OCRREADER_H
#define OCRREADER_H

#include <QObject>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


class OcrReader : public QObject
{
    Q_OBJECT
public:
    OcrReader(QObject* parent = nullptr) : QObject(parent) {}

    QString runTesseract(const QString& imagePath, const QString& lang = "rus")
    {
        // исправить на кросплатформеный вариант
        qputenv("TESSDATA_PREFIX", "/home/sysadm/qt_project/speaker/tesseract/tessdata/");


        tesseract::TessBaseAPI api;
        if (api.Init(NULL, lang.toStdString().c_str())) {
            emit alert("OcrReader::runTesseract", tr("Could not initialize tesseract"), "error");
            return QString();
        }

        Pix* image = pixRead(imagePath.toStdString().c_str());
        api.SetImage(image);
        char* outText = api.GetUTF8Text();

        QString result = QString::fromUtf8(outText);

        delete [] outText;
        pixDestroy(&image);
        api.End();

        return result;
    }

signals:
    void                    alert(const QString & title, const QString & text, QString type);
};

#endif // OCRREADER_H
