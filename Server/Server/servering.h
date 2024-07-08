#ifndef SERVERING_H
#define SERVERING_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>

class Servering : public QWidget {
    Q_OBJECT

public:
    explicit Servering(QWidget *parent = nullptr); // 构造函数
    bool startServer(const QString &ipAddress, quint16 port); // 启动服务器
    QString getError() const; // 获取最后的错误信息

private slots:
    void incomingConnection(); // 处理新连接
    void readClient(); // 读取客户端消息
    void clientDisconnected(); // 处理客户端断开连接

private:
    QTcpServer *server; // 服务器对象
    QTcpSocket *clientSocket1 = nullptr, *clientSocket2 = nullptr; // 两个客户端连接
    QTextEdit *infoDisplay; // 信息显示文本框
    QString lastError; // 最后错误信息

    void distributeColor(QTcpSocket *clientSocket, const QString &color); // 分配颜色
    void broadcastMove(int x, int y, int color); // 广播棋步
    void handleUndoRequest(QTcpSocket *clientSocket); // 处理悔棋请求
    void forwardUndoResponse(QTcpSocket *sourceClient, const QString &message); // 转发悔棋响应
    void notifyOpponentLeft(QTcpSocket *remainingClient); // 通知对手已离开
    void handleDrawRequest(QTcpSocket *clientSocket); // 处理求和请求
    void handleSurrender(QTcpSocket *clientSocket); // 处理认输请求
    void forwardDrawResponse(QTcpSocket *sourceClient, const QString &message); // 转发求和响应
    void handleSwapColorRequest(QTcpSocket *clientSocket); // 处理交换颜色请求
    void forwardSwapColorResponse(QTcpSocket *sourceClient, const QString &message); // 转发交换颜色响应
};

#endif // SERVERING_H
