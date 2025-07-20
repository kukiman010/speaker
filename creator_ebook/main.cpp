#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QSharedMemory>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedMemory sharedMemory("MyUniqueAppKeyForSpeaker_creator_ebook");
    // Пробуем зааттачиться к старому сегменту, если получится — удаляем
    if (sharedMemory.attach()) {
        sharedMemory.detach();
    }

    // Теперь пытаемся создать заново
    if (!sharedMemory.create(1)) {
        QMessageBox::critical(nullptr, QObject::tr("Ошибка"), QObject::tr("Программа уже запущена!"));
        return 1;
    }



    MainWindow w;
    w.show();

    return a.exec();
}
