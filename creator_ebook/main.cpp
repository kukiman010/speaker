#include "mainwindow.h"
#include <QApplication>

// #include <tesseract/baseapi.h>
// #include <leptonica/allheaders.h>

// QString ocr_image(const QString &imagePath, const QString &lang = "rus+eng") {
//     tesseract::TessBaseAPI api;
//     if (api.Init(nullptr, lang.toStdString().c_str())) {
//         // qDebug() << "Could not initialize tesseract.";
//         return QString();
//     }
//     Pix *image = pixRead(imagePath.toStdString().c_str());
//     api.SetImage(image);
//     QString result = QString::fromUtf8(api.GetUTF8Text());
//     api.End();
//     pixDestroy(&image);
//     return result;
// }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
