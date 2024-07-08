#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include "mutiwindow.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    void startServer();
    void sendMoveToClients(int x, int y, int color);

private slots:
    void newConnection();
    void readClientData();

private:
    QTcpSocket *client1;
    QTcpSocket *client2;
    QVector<QTcpSocket*> clients;
    MutiWindow game;

    void sendToClient(QTcpSocket *client, const QString &message);
    void handleMove(int x, int y, int color);
};

#endif // SERVER_H
