#include "mainwindow.h"

#include <QApplication>
#include "socketclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    SocketClient client;
    client.connectToServer("127.0.0.1", 1234);
    client.sendMessage("Привет, сервер!");


    return a.exec();
}
