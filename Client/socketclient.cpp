#include "socketclient.h"

#include <QDataStream>

SocketClient::SocketClient(QObject *parent) : QObject(parent)
{
    // Установите ключ шифрования
    encryptionKey = "Мой секретный ключ";

    // socket = new QTcpSocket(this);
    socket = QSharedPointer<QTcpSocket>(new QTcpSocket(this));
    connect(socket.data(), &QTcpSocket::readyRead, this, &SocketClient::onReadyRead);
    connect(socket.data(), &QTcpSocket::disconnected, this, &SocketClient::onDisconnect);
}

void SocketClient::connectToServer(const QString &host, quint16 port)
{
    socket->connectToHost(host, port);
}

void SocketClient::sendMessage(const QString &message)
{
    // Зашифровать сообщение
    // QByteArray encryptedData = QCryptographicHash::hash(message.toUtf8(), QCryptographicHash::Sha256);
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << message;
    // clientSocket->write(data);

    socket->write(data);
}

void SocketClient::onReadyRead()
{
    // Получить сообщение от сервера
    QByteArray data = socket->readAll();

    // Расшифровать сообщение
    // QByteArray decryptedData = QCryptographicHash::hash(data, QCryptographicHash::Sha256);

    // // Обработать сообщение
    // QString message = QString::fromUtf8(decryptedData);
    // qDebug() << "Сообщение от сервера: " << message;
    QString message = QString::fromUtf8(data);
    qDebug() << "Сообщение от сервера: " << message;
}

void SocketClient::onDisconnect()
{
    qDebug() << "Отключен от сервера";
}

// QString SocketClient::decryptMessage(const QString &encryptedMessage)
// {
//     // Создать объект QCryptographicCipher
//     QCryptographicCipher cipher(QCryptographicCipher::Blowfish);
//     cipher.setKey(encryptionKey.toUtf8());
//     cipher.setOperation(QCryptographicCipher::Decrypt);

//     // Расшифровать сообщение
//     QByteArray decryptedMessage = cipher.process(encryptedMessage.toUtf8());

//     // Вернуть расшифрованное сообщение
//     return QString::fromUtf8(decryptedMessage);
// }


