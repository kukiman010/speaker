#include "socketserver.h"

#include <QDataStream>
#include <QUuid>

SocketServer::SocketServer(QObject *parent) : QTcpServer(parent)
{
    // Установите ключ шифрования
    encryptionKey = "Мой секретный ключ";
    _blockSize = 0;
}

SocketServer::~SocketServer()
{
    clientIds.clear();

    QHashIterator<QString, QSharedPointer<QTcpSocket>> i(clients);
    while (i.hasNext())
    {
        i.next();
        i.value()->disconnect();  // Disconnect any signals and slots
        i.value()->deleteLater(); // Delete the socket using Qt's deleteLater
    }
    clients.clear();
}

void SocketServer::incomingConnection(qintptr socketDescriptor)// Принять новое соединение
{
    QSharedPointer<QTcpSocket> clientSocket = QSharedPointer<QTcpSocket>(new QTcpSocket(this));
    clientSocket->setSocketDescriptor(socketDescriptor);

    // Сгенерировать уникальный идентификатор клиента
//    QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QString uuid = QUuid::createUuid().toString();
    clientIds.insert(socketDescriptor, uuid);
    clients.insert(uuid, clientSocket);

    // Подключить сигналы и слоты
    connect(clientSocket.data(), &QTcpSocket::readyRead, this, &SocketServer::onReadyRead);
    connect(clientSocket.data(), &QTcpSocket::disconnected, this, &SocketServer::onDisconnected);

    // Отправить идентификатор клиента новому клиенту
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << uuid;
    clientSocket->write(data);

    qDebug() << "Новый клиент подключен: " << uuid;
}

void SocketServer::onReadyRead()
{
    // if ( !_socket.data() )
        // return;

    QTcpSocket *clientSocket = static_cast<QTcpSocket *>(sender());

    QByteArray arrN;
    QDataStream streamN( &arrN, QIODevice::ReadOnly );

    if ( 0 == _blockSize && clientSocket->bytesAvailable() < 4 )
        return;

    if ( 0 == _blockSize )
    {
        arrN = clientSocket->read( sizeof(int) );
        streamN >> _blockSize;
    }
    if ( clientSocket->bytesAvailable() < _blockSize )
        return;


    QByteArray arr = clientSocket->read( _blockSize );
    _blockSize = 0 ;

    QDataStream stream( &arr, QIODevice::ReadOnly );

    AbstractCommand *cmd = nullptr;
    stream >> cmd;

    if ( !cmd )
        return;

    // emit toQueue( TFuncs::make_shared( cmd ) );

    if ( clientSocket->bytesAvailable() > 0 )
        onReadyRead();

    // // Получить сообщение от клиента
    // QTcpSocket *clientSocket = static_cast<QTcpSocket *>(sender());
    // QByteArray data = clientSocket->readAll();

    // // Расшифровать сообщение
    // // QByteArray decryptedData = QCryptographicHash::hash(data, QCryptographicHash::Sha256);

    // // Обработать сообщение
    // // QString message = QString::fromUtf8(decryptedData);
    // QString message = QString::fromUtf8(data);
    // qDebug() << "Сообщение от клиента " << clientIds[clientSocket->socketDescriptor()] << ": " << message;
}

void SocketServer::onDisconnected()
{
    // Get the socket that disconnected
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());

    // Remove client from the lists
    QString clientId = clientIds.value(clientSocket->socketDescriptor());
    clientIds.remove(clientSocket->socketDescriptor());
    clients.remove(clientId);

    qDebug() << "Client " << clientId << " has disconnected";
}

// #include "abstractcommand.h"

void SocketServer::onSend(CmdPtr cmd)
{
    QSharedPointer<QTcpSocket> clientSocket = clients.value(cmd->userId());
    // armId

    if (!clientSocket)
        return;

    QByteArray arr;
    QDataStream stream( &arr, QIODevice::WriteOnly );
    AbstractCommand * command = cmd.data();
    // AbstractCommand * command = nullptr;
    // AbstractCommand command();
    stream << command;

    int size = arr.size();
    QByteArray num;
    QDataStream strm( &num, QIODevice::WriteOnly );
    strm << size;

    clientSocket->write( num );
    clientSocket->write( arr );
}

// void SocketServer::onSend(CmdPtr cmd)
// {
//     QSharedPointer<QTcpSocket> clientSocket = clients.value(cmd->userId());

//     if (!clientSocket)
//         return;

//     QByteArray arr;
//     QDataStream stream( &arr, QIODevice::WriteOnly );
//     // AbstractCommand * command = cmd.data();
//     AbstractCommand * command = new AbstractCommand();
//     stream << command;

//     int size = arr.size();
//     QByteArray num;
//     QDataStream strm( &num, QIODevice::WriteOnly );
//     strm << size;

//     clientSocket->write( num );
//     clientSocket->write( arr );
// }

