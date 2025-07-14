#include "ocrreader.h"

OcrReader::OcrReader(QObject* parent)
    : QObject(parent)
{

}

QString OcrReader::runTesseract(const QString &imagePath, const QString &lang)
{
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


