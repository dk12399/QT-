#include "servering.h"
#include <QVBoxLayout>

Servering::Servering(QWidget *parent) : QWidget(parent), server(new QTcpServer(this)) {
    setFixedSize(400, 300); // 设置窗口大小
    setWindowTitle("服务器窗口"); // 设置窗口标题
    infoDisplay = new QTextEdit(this); // 创建信息显示文本框
    infoDisplay->setReadOnly(true); // 设置为只读

    QVBoxLayout *layout = new QVBoxLayout(this); // 创建布局
    layout->addWidget(infoDisplay); // 将文本框添加到布局

    connect(server, &QTcpServer::newConnection, this, &Servering::incomingConnection); // 连接新连接信号和槽
}

bool Servering::startServer(const QString &ipAddress, quint16 port) {
    if (!server->listen(QHostAddress(ipAddress), port)) { // 尝试启动服务器
        lastError = server->errorString(); // 保存错误信息
        infoDisplay->append("启动服务器错误: " + lastError); // 显示错误信息
        return false;
    }
    infoDisplay->append("服务器启动成功: " + ipAddress + ":" + QString::number(port)); // 显示成功信息
    return true;
}

void Servering::incomingConnection() {
    QTcpSocket *clientSocket = server->nextPendingConnection(); // 获取新连接的客户端套接字
    if (!clientSocket1) { // 如果第一个客户端套接字为空
        clientSocket1 = clientSocket; // 设置第一个客户端套接字
        distributeColor(clientSocket1, "Black"); // 分配黑色
        connect(clientSocket1, &QTcpSocket::readyRead, this, &Servering::readClient); // 连接读取信号和槽
        connect(clientSocket1, &QTcpSocket::disconnected, this, &Servering::clientDisconnected); // 连接断开信号和槽
    } else if (!clientSocket2) { // 如果第二个客户端套接字为空
        clientSocket2 = clientSocket; // 设置第二个客户端套接字
        distributeColor(clientSocket2, "White"); // 分配白色
        connect(clientSocket2, &QTcpSocket::readyRead, this, &Servering::readClient); // 连接读取信号和槽
        connect(clientSocket2, &QTcpSocket::disconnected, this, &Servering::clientDisconnected); // 连接断开信号和槽

        // 通知两个客户端对方已连接
        clientSocket1->write("OpponentConnected\n");
        clientSocket2->write("OpponentConnected\n");
    } else {
        clientSocket->disconnectFromHost(); // 如果已有两个客户端连接，则断开新的连接
    }
}

void Servering::distributeColor(QTcpSocket *clientSocket, const QString &color) {
    qDebug() << "分配颜色 " << color << " 给客户端 " << clientSocket;
    clientSocket->write(color.toUtf8() + "\n"); // 向客户端发送颜色信息
    infoDisplay->append("分配 " + color + " 给一个玩家。");
    if (color == "Black") { // 如果分配的是黑色
        if (clientSocket == clientSocket2) { // 如果第二个客户端是黑色，则交换客户端
            std::swap(clientSocket1, clientSocket2);
        }
    } else if (color == "White") { // 如果分配的是白色
        if (clientSocket == clientSocket1) { // 如果第一个客户端是白色，则交换客户端
            std::swap(clientSocket1, clientSocket2);
        }
    }
}

void Servering::readClient() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender()); // 获取发送信号的客户端套接字
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll(); // 读取客户端发送的数据
    QString message = QString::fromUtf8(data.trimmed()); // 转换为字符串

    qDebug() << "Server received message:" << message;

    if (message == "UndoRequest") { // 处理悔棋请求
        handleUndoRequest(clientSocket);
    } else if (message.startsWith("UndoAccepted") || message.startsWith("UndoRejected")) { // 处理悔棋响应
        forwardUndoResponse(clientSocket, message);
    } else if (message == "OpponentLeft") { // 处理对手离开消息
        QTcpSocket *otherClient = (clientSocket == clientSocket1) ? clientSocket2 : clientSocket1;
        notifyOpponentLeft(otherClient);
    } else if (message == "DrawRequest") { // 处理求和请求
        handleDrawRequest(clientSocket);
    } else if (message.startsWith("DrawAccepted") || message.startsWith("DrawRejected")) { // 处理求和响应
        forwardDrawResponse(clientSocket, message);
    } else if (message == "Surrender") { // 处理认输消息
        handleSurrender(clientSocket);
    } else if (message == "SwapColorRequest") { // 处理交换颜色请求
        handleSwapColorRequest(clientSocket);
    } else if (message.startsWith("SwapColorAccepted") || message.startsWith("SwapColorRejected")) { // 处理交换颜色响应
        forwardSwapColorResponse(clientSocket, message);
    } else if (message.startsWith("CHAT:")) { // 处理聊天消息
        QString chatMsg = message.mid(5);
        QTcpSocket *otherClient = (clientSocket == clientSocket1) ? clientSocket2 : clientSocket1;
        if (otherClient) {
            otherClient->write("CHAT:" + chatMsg.toUtf8() + "\n");
        }
    } else {
        QStringList parts = message.split(",");
        if (parts.size() == 3) { // 处理棋步消息
            int x = parts[0].toInt();
            int y = parts[1].toInt();
            int color = parts[2].toInt();
            broadcastMove(x, y, color);
        }
    }
}

void Servering::forwardUndoResponse(QTcpSocket *sourceClient, const QString &message) {
    QTcpSocket *otherClient = (sourceClient == clientSocket1) ? clientSocket2 : clientSocket1;
    if (otherClient && otherClient->isOpen()) { // 如果对方客户端存在且连接中
        qDebug() << "将悔棋请求转发到其他客户端";
        otherClient->write(message.toUtf8() + "\n");
    } else {
        qDebug() << "没有其他客户端可转发到或socket未打开";
    }
}

void Servering::handleUndoRequest(QTcpSocket *clientSocket) {
    QTcpSocket *otherSocket = (clientSocket == clientSocket1) ? clientSocket2 : clientSocket1;
    if (otherSocket) {
        otherSocket->write("UndoRequest\n");
    }
}

void Servering::handleDrawRequest(QTcpSocket *clientSocket) {
    QTcpSocket *otherSocket = (clientSocket == clientSocket1) ? clientSocket2 : clientSocket1;
    if (otherSocket) {
        otherSocket->write("DrawRequest\n");
    }
}

void Servering::forwardDrawResponse(QTcpSocket *sourceClient, const QString &message) {
    QTcpSocket *otherClient = (sourceClient == clientSocket1) ? clientSocket2 : clientSocket1;
    if (otherClient && otherClient->isOpen()) { // 如果对方客户端存在且连接中
        qDebug() << "转发平局请求到另一个客户端";
        otherClient->write(message.toUtf8() + "\n");
    } else {
        qDebug() << "没有客户端可转发或者socket未打开";
    }
}

void Servering::handleSurrender(QTcpSocket *clientSocket) {
    QTcpSocket *otherSocket = (clientSocket == clientSocket1) ? clientSocket2 : clientSocket1;
    if (otherSocket) {
        otherSocket->write("Surrender\n");
    }
}

void Servering::handleSwapColorRequest(QTcpSocket *clientSocket) {
    QTcpSocket *otherSocket = (clientSocket == clientSocket1) ? clientSocket2 : clientSocket1;
    if (otherSocket) {
        otherSocket->write("SwapColorRequest\n");
    }
}

void Servering::forwardSwapColorResponse(QTcpSocket *sourceClient, const QString &message) {
    QTcpSocket *otherClient = (sourceClient == clientSocket1) ? clientSocket2 : clientSocket1;
    if (otherClient && otherClient->isOpen()) { // 如果对方客户端存在且连接中
        qDebug() << "转发发送颜色请求给另一个客户端";
        otherClient->write(message.toUtf8() + "\n");
        if (message == "SwapColorAccepted\n") { // 如果交换颜色请求被接受
            qDebug() << "Swapping colors between clients and notifying clients to reset boards";
            // 交换颜色并通知客户端重置棋盘
            distributeColor(clientSocket1, clientSocket1 == sourceClient ? "White" : "Black");
            distributeColor(clientSocket2, clientSocket2 == sourceClient ? "White" : "Black");
            clientSocket1->write("ResetBoard\n");
            clientSocket2->write("ResetBoard\n");
        }
    } else {
        qDebug() << "没有客户端可转发或者socket未打开";
    }
}

void Servering::broadcastMove(int x, int y, int color) {
    QString move = QString::number(x) + "," + QString::number(y) + "," + QString::number(color) + "\n";
    if (clientSocket1) clientSocket1->write(move.toUtf8());
    if (clientSocket2) clientSocket2->write(move.toUtf8());
    infoDisplay->append("广播棋步: " + move); // 显示广播的棋步
}

void Servering::clientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket) return;

    if (clientSocket == clientSocket1) { // 判断断开的客户端
        clientSocket1 = nullptr;
        infoDisplay->append("玩家 Black 已断开连接。");
    } else if (clientSocket == clientSocket2) {
        clientSocket2 = nullptr;
        infoDisplay->append("玩家 White 已断开连接。");
    }

    QTcpSocket *remainingClient = (clientSocket == clientSocket1) ? clientSocket2 : clientSocket1;
    if (remainingClient) {
        notifyOpponentLeft(remainingClient); // 通知剩余客户端对手已离开
    }

    clientSocket->deleteLater();
}

void Servering::notifyOpponentLeft(QTcpSocket *remainingClient) {
    if (remainingClient && remainingClient->isOpen()) {
        remainingClient->write("OpponentLeft\n");
    }
}

QString Servering::getError() const {
    return lastError;
}
