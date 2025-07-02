#include "bookconvector.h"
#include <QFileInfo>

BookConvector::BookConvector(QObject *parent)
    : QObject{parent}
{}

void BookConvector::conver(QString filePath)
{
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix();

    if( extension == "pdf")
    {

    }
    else
        emit alert(tr("Ошибка формата файла"), tr("в данный момент программа не умеет работать с ") + extension, "warning");
}
