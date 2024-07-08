#include "server.h"
#include <QDataStream>
#include <QDebug>

Server::Server(QObject *parent) :
    QTcpServer(parent), client1(nullptr), client2(nullptr)
{
    connect(this, &Server::newConnection, this, &Server::newConnection);
}

void Server::startServer()
{
    int port = 1234;
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port" << port;
    } else {
        qDebug() << "Failed to start server:" << this->errorString();
    }
}

void Server::newConnection()
{
    QTcpSocket *client = this->nextPendingConnection();
    clients.append(client);
    qDebug() << "New client connected from" << client->peerAddress().toString();

    connect(client, &QTcpSocket::readyRead, this, &Server::readClientData);

    if (clients.size() == 1) {
        client1 = client;
        sendToClient(client1, "You are Player 1 (Black)");
    } else if (clients.size() == 2) {
        client2 = client;
        sendToClient(client2, "You are Player 2 (White)");
        sendToClient(client1, "Game start. It's your turn!");
        sendToClient(client2, "Game start. Waiting for Player 1 to move.");
    } else {
        sendToClient(client, "Server is full.");
        client->disconnectFromHost();
    }
}

void Server::readClientData()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    QDataStream in(client);
    int x, y, color;
    in >> x >> y >> color;

    if (client == client1) {
        handleMove(x, y, 1); // Player 1 (Black)
    } else if (client == client2) {
        handleMove(x, y, 2); // Player 2 (White)
    }
}

void Server::sendMoveToClients(int x, int y, int color)
{
    QString message = QString("MOVE %1 %2 %3").arg(x).arg(y).arg(color);
    sendToClient(client1, message);
    sendToClient(client2, message);
}

void Server::sendToClient(QTcpSocket *client, const QString &message)
{
    if (client && client->state() == QAbstractSocket::ConnectedState) {
        QDataStream out(client);
        out << message;
    }
}

void Server::handleMove(int x, int y, int color)
{
    if (game.isValidMove(x, y, color)) {
        game.map[x][y] = color;
        game.executeFlip(x, y, color);
        game.update();
        sendMoveToClients(x, y, color);

        game.checkGameStateAndProceed();
    }
}
