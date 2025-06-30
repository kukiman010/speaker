#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
//#include <QCryptographicHash>
#include <QDebug>



// #include "command.h"
#include "abstractcommand.h"

//class SocketServer : public QTcpServer
class SocketServer : public QTcpServer
{
    Q_OBJECT
public:
    SocketServer(QObject *parent = nullptr);
    ~SocketServer();

protected:
    void                                            incomingConnection(qintptr socketDescriptor) override;

public slots:
    void                                            onReadyRead();
    void                                            onDisconnected();
    void                                            onSend(CmdPtr cmd);

private:
    QMap<qintptr, QString>                          clientIds;  // Сопоставление дескрипторов сокетов с идентификаторами клиентов
    QHash<QString, QSharedPointer<QTcpSocket> >     clients;  // Сопоставление идентификаторов клиентов с сокетами
    QByteArray                                      encryptionKey;  // Ключ шифрования
    int                                             _blockSize;
};

#endif // SOCKETSERVER_H
