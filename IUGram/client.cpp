#include <QtNetwork>

#include "client.h"
#include "connection.h"
#include "peermanager.h"

Client::Client()
{
    peerManager = new PeerManager(this);
    peerManager->setServerPort(server.serverPort());
    peerManager->startBroadcasting();

    // Коннекты определяющие новые подключения к чату
    QObject::connect(peerManager, SIGNAL(newConnection(Connection*)), this, SLOT(newConnection(Connection*)));
    QObject::connect(&server, SIGNAL(newConnection(Connection*)), this, SLOT(newConnection(Connection*)));
}

// Отправка сообщения
void Client::sendMessage(const QString &message)
{
    if (message.isEmpty())
    {
        return;
    }

    // Список подключений
    QList<Connection *> connections = peers.values();
    foreach (Connection *connection, connections)
    {
         connection->sendMessage(message);
    }
}

// Информация с никнеймом, хостом и портом пользователя
QString Client::nickName() const
{
    return "< " + QString(peerManager->userName()) + ">:" + QHostInfo::localHostName()
            + ':' + QString::number(server.serverPort());
}

// Определение подключения
bool Client::hasConnection(const QHostAddress &senderIp, int senderPort) const
{
    if (senderPort == -1)
    {
        return peers.contains(senderIp);
    }

    if (!peers.contains(senderIp))
    {
        return false;
    }

    QList<Connection *> connections = peers.values(senderIp);
    foreach (Connection *connection, connections)
    {
        if (connection->peerPort() == senderPort)
        {
            return true;
        }
    }

    return false;
}

// Осуществление нового подключения
void Client::newConnection(Connection *connection)
{
    connection->setGreetingMessage(peerManager->userName());

    // Коннекты для определения ошибки, отключения и готовности к чтению
    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));
}

// Осуществление чтения сообщения
void Client::readyForUse()
{
    Connection *connection = qobject_cast<Connection *>(sender());
    // Если нет подключения
    if (!connection || hasConnection(connection->peerAddress(), connection->peerPort()))
    {
        return;
    }
    // Коннект по определению нового сообщения
    connect(connection, SIGNAL(newMessage(QString,QString)), this, SIGNAL(newMessage(QString,QString)));

    peers.insert(connection->peerAddress(), connection);
    QString nick = connection->name();
    if (!nick.isEmpty())
    {
        emit newParticipant(nick);
    }
}

// Осуществление отключения
void Client::disconnected()
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
    {
        removeConnection(connection);
    }
}

// Обработка ошибки подключения
void Client::connectionError(QAbstractSocket::SocketError /* socketError */)
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
    {
        removeConnection(connection);
    }
}

// Удаление подключения
void Client::removeConnection(Connection *connection)
{
    if (peers.contains(connection->peerAddress()))
    {
        peers.remove(connection->peerAddress());
        QString nick = connection->name();
        if (!nick.isEmpty())
        {
            emit participantLeft(nick);
        }
    }
    connection->deleteLater();
}
