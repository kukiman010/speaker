#include "socket.h"

// NetworkSocket::NetworkSocket(TypeNetworkSocket type, QObject *parent) : QObject(parent), type(type) {
//     socket = new QTcpSocket(this);
//     connect(socket, &QTcpSocket::connected, this, &NetworkSocket::onConnected);
//     connect(socket, &QTcpSocket::disconnected, this, &NetworkSocket::onDisconnected);
//     connect(socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &NetworkSocket::onError);
//     connect(socket, &QTcpSocket::readyRead, this, &NetworkSocket::onReadyRead);
//     connect(socket, &QTcpSocket::stateChanged, this, &NetworkSocket::onStateChanged);
// }

// NetworkSocket::~NetworkSocket() {
//     socket->deleteLater();
// }

// bool NetworkSocket::connectToHost(const QString &host, quint16 port) {
//     return socket->connectToHost(host, port);
// }

// bool NetworkSocket::listen(quint16 port) {
//     return socket->listen(port);
// }

// void NetworkSocket::disconnect() {
//     socket->disconnect();
// }

// void NetworkSocket::close() {
//     socket->close();
// }

// bool NetworkSocket::reconnect() {
//     return socket->reconnect();
// }

// QByteArray NetworkSocket::encrypt(const QByteArray &data) {
//     QCryptographicHash hash(QCryptographicHash::Sha256);
//     hash.addData(encryptionKey);
//     QByteArray key = hash.result();
//     QByteArray encryptedData = data;
//     for (int i = 0; i < data.size(); i++) {
//         encryptedData[i] ^= key[i % key.size()];
//     }
//     return encryptedData;
// }

// QByteArray NetworkSocket::decrypt(const QByteArray &data) {
//     QCryptographicHash hash(QCryptographicHash::Sha256);
//     hash.addData(encryptionKey);
//     QByteArray key = hash.result();
//     QByteArray decryptedData = data;
//     for (int i = 0; i < data.size(); i++) {
//         decryptedData[i] ^= key[i % key.size()];
//     }
//     return decryptedData;
// }

// void NetworkSocket::onConnected() {
//     emit connected();
// }

// void NetworkSocket::onDisconnected() {
//     emit disconnected();
// }

// void NetworkSocket::onError(QAbstractSocket::SocketError error) {
//     emit error(error);
// }

// void NetworkSocket::onReadyRead() {
//     QByteArray data = socket->readAll();
//     emit dataReceived(decrypt(data));
// }

// void NetworkSocket::onStateChanged(QAbstractSocket::SocketState state) {
//     if (state == QAbstractSocket::UnconnectedState) {
//         emit disconnected();
//     }
// }
