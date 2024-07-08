#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>
#include <QHostAddress>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    void connectToServer(const QHostAddress &address, quint16 port);
    void sendMove(int x, int y, int color);

signals:
    void moveReceived(int x, int y, int color);
    void connected();
    void disconnected();
    void errorOccurred(const QString &errorString);

private slots:
    void readServerData();
    void handleConnection();
    void handleDisconnection();
    void handleError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H
