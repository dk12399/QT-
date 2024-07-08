#include "client.h"
#include <QDataStream>
#include <QDebug>

Client::Client(QObject *parent)
    : QObject(parent), socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::readyRead, this, &Client::readServerData);
    connect(socket, &QTcpSocket::connected, this, &Client::handleConnection);
    connect(socket, &QTcpSocket::disconnected, this, &Client::handleDisconnection);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &Client::handleError);
}

void Client::connectToServer(const QHostAddress &address, quint16 port)
{
    socket->connectToHost(address, port);
}

void Client::sendMove(int x, int y, int color)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QDataStream out(socket);
        out << x << y << color;
    }
}

void Client::readServerData()
{
    QDataStream in(socket);
    QString message;
    in >> message;

    if (message.startsWith("MOVE")) {
        QStringList parts = message.split(" ");
        int x = parts[1].toInt();
        int y = parts[2].toInt();
        int color = parts[3].toInt();
        emit moveReceived(x, y, color);
    }
}

void Client::handleConnection()
{
    emit connected();
}

void Client::handleDisconnection()
{
    emit disconnected();
}

void Client::handleError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    emit errorOccurred(socket->errorString());
}
