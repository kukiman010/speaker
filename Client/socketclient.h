#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

// class SocketClient
#include <QTcpSocket>
#include <QCryptographicHash>
#include <QDebug>

class SocketClient : public QObject
{
    Q_OBJECT
public:
    SocketClient(QObject *parent = nullptr);
    // QString decryptMessage(const QString &encryptedMessage);

public slots:
    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QString &message);
    void onReadyRead();
    void onDisconnect();

private:
    QSharedPointer<QTcpSocket>      socket;
    QByteArray                      encryptionKey;  // Ключ шифрования
    int                             clientId;
};
#endif // SOCKETCLIENT_H
