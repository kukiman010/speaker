// class NetworkServer : public QObject {
// class NetworkConnection : public QObject
#include <QTcpSocket>
#include <QTcpServer>
#include <QCryptographicHash>

// enum class TypeNetworkSocket {
//     TNS_Server,
//     TNS_Client
// };

// class NetworkSocket : public QObject {
//     Q_OBJECT
// public:
//     NetworkSocket(TypeNetworkSocket type, QObject *parent = nullptr);
//     ~NetworkSocket();

//     // Подключение
//     bool connectToHost(const QString &host, quint16 port);
//     bool listen(quint16 port);

//     // Отключение
//     void disconnect();
//     void close();

//     // Переподключение
//     bool reconnect();

//     // Шифрование
//     QByteArray encrypt(const QByteArray &data);
//     QByteArray decrypt(const QByteArray &data);

// signals:
//     void connected();
//     void disconnected();
//     void error(QAbstractSocket::SocketError);
//     void dataReceived(const QByteArray &data);

// protected slots:
//     void onConnected();
//     void onDisconnected();
//     void onError(QAbstractSocket::SocketError);
//     void onReadyRead();
//     void onStateChanged(QAbstractSocket::SocketState);

// private:
//     QTcpSocket *socket;
//     QByteArray encryptionKey;
//     TypeNetworkSocket type;
// };
