#include "mutiwindow.h"
#include "ui_mutiwindow.h"
#include "mainwindow.h"

MutiWindow::MutiWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MutiWindow) {
    ui->setupUi(this);
    setWindowTitle("黑白棋客户端-双人对战模式");
    setFixedSize(880, 680); // 调整窗口大小以适应聊天框和标签
    xOffset = 20;
    yOffset = 20;
    this->setAutoFillBackground(true);
    this->setMouseTracking(true);
    resetBoard();
    setupUI();

    // 初始化 QTcpSocket 对象
    socket = new QTcpSocket(this);

    // 连接信号和槽
    connect(socket, &QTcpSocket::connected, this, &MutiWindow::onConnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &MutiWindow::onErrorOccurred);
    connect(socket, &QTcpSocket::readyRead, this, &MutiWindow::readServer);
}

void MutiWindow::onConnected() {
    connectionStatus->setText("连接成功");
    qDebug() << "Connection established.";
}

void MutiWindow::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    connectionStatus->setText("连接失败: " + socket->errorString());
    qDebug() << "Connection error:" << socket->errorString();
}

void MutiWindow::setupUI() {
    // 设置棋盘区域
    QWidget *boardWidget = new QWidget(this);
    boardWidget->setFixedSize(boardWidth + 2 * xOffset, boardHeight + 2 * yOffset);

    // 创建右侧控件布局
    QVBoxLayout *rightLayout = new QVBoxLayout;

    // 网络设置组
    QGroupBox *networkGroupBox = new QGroupBox(tr("网络设置"), this);
    QVBoxLayout *networkVBox = new QVBoxLayout;

    QLineEdit *ipInput = new QLineEdit;
    ipInput->setPlaceholderText(tr("输入IP地址"));
    networkVBox->addWidget(ipInput);

    QLineEdit *portInput = new QLineEdit;
    portInput->setPlaceholderText(tr("输入端口"));
    networkVBox->addWidget(portInput);

    QPushButton *connectBtn = new QPushButton(tr("连接"));
    networkVBox->addWidget(connectBtn);

    connectionStatus = new QLabel("等待连接", this);
    networkVBox->addWidget(connectionStatus);

    networkGroupBox->setLayout(networkVBox);
    rightLayout->addWidget(networkGroupBox);

    // 游戏统计组
    QGroupBox *infoGroupBox = new QGroupBox(tr("游戏统计"), this);
    QVBoxLayout *infoVBox = new QVBoxLayout;

    colorLabel = new QLabel("颜色: 未知", this);
    infoVBox->addWidget(colorLabel);

    blackCountLabel = new QLabel("黑棋数量: 0", this);
    infoVBox->addWidget(blackCountLabel);

    whiteCountLabel = new QLabel("白棋数量: 0", this);
    infoVBox->addWidget(whiteCountLabel);

    turnLabel = new QLabel("当前回合: 未知", this);
    infoVBox->addWidget(turnLabel);

    infoGroupBox->setLayout(infoVBox);
    rightLayout->addWidget(infoGroupBox);

    // 游戏控制组
    QGroupBox *controlGroupBox = new QGroupBox(tr("游戏控制"), this);
    QVBoxLayout *controlVBox = new QVBoxLayout;

    QPushButton *undoBtn = new QPushButton(tr("悔棋"), this);
    controlVBox->addWidget(undoBtn);
    connect(undoBtn, &QPushButton::clicked, this, &MutiWindow::handleUndoRequest);

    QPushButton *drawBtn = new QPushButton(tr("求和"), this); // 添加求和按钮
    controlVBox->addWidget(drawBtn);
    connect(drawBtn, &QPushButton::clicked, this, &MutiWindow::handleDrawRequest);

    QPushButton *surrenderBtn = new QPushButton(tr("认输"), this); // 添加认输按钮
    controlVBox->addWidget(surrenderBtn);
    connect(surrenderBtn, &QPushButton::clicked, this, &MutiWindow::handleSurrender);

    QPushButton *swapColorBtn = new QPushButton(tr("交换颜色"), this); // 添加交换颜色按钮
    controlVBox->addWidget(swapColorBtn);
    connect(swapColorBtn, &QPushButton::clicked, this, &MutiWindow::handleSwapColorRequest);

    QPushButton *returnBtn = new QPushButton(tr("返回主页面"), this);
    controlVBox->addWidget(returnBtn);
    connect(returnBtn, &QPushButton::clicked, this, &MutiWindow::handleReturnToMain);

    controlGroupBox->setLayout(controlVBox);
    rightLayout->addWidget(controlGroupBox);

    // 聊天组
    QGroupBox *chatGroupBox = new QGroupBox(tr("聊天"), this);
    QVBoxLayout *chatVBox = new QVBoxLayout;

    chatBox = new QTextEdit(this);
    chatBox->setReadOnly(true);
    chatBox->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 设置聊天框可以用鼠标滚轮翻动
    chatBox->setFixedHeight(140); // 设置聊天框高度
    chatVBox->addWidget(chatBox);

    QHBoxLayout *chatHBox = new QHBoxLayout;
    chatInput = new QLineEdit(this);
    chatHBox->addWidget(chatInput);

    QPushButton *sendBtn = new QPushButton(tr("发送"), this);
    chatHBox->addWidget(sendBtn);
    connect(sendBtn, &QPushButton::clicked, this, &MutiWindow::sendMessage);

    chatVBox->addLayout(chatHBox);
    chatGroupBox->setLayout(chatVBox);
    rightLayout->addWidget(chatGroupBox);

    rightLayout->addStretch();

    // 设置右侧控件区域的宽度
    QWidget *rightWidget = new QWidget(this);
    rightWidget->setFixedWidth(200);
    rightWidget->setLayout(rightLayout);

    // 创建主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(boardWidget);
    mainLayout->addWidget(rightWidget);

    setLayout(mainLayout);

    connect(connectBtn, &QPushButton::clicked, this, [this, ipInput, portInput]() {
        QString ip = ipInput->text();
        int port = portInput->text().toInt();
        if (port == 0 && !portInput->text().isEmpty()) {
            connectionStatus->setText("端口号输入错误");
            return;
        }
        this->attemptConnection(ip, port);
    });
}






void MutiWindow::attemptConnection(const QString &ip, int port) {
    qDebug() << "Attempting to connect to" << ip << "on port" << port;
    socket->abort();

    // 断开之前的连接
    disconnect(socket, &QTcpSocket::connected, this, &MutiWindow::onConnected);
    disconnect(socket, &QTcpSocket::errorOccurred, this, &MutiWindow::onErrorOccurred);
    disconnect(socket, &QTcpSocket::readyRead, this, &MutiWindow::readServer);

    // 重新连接信号和槽
    connect(socket, &QTcpSocket::connected, this, &MutiWindow::onConnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &MutiWindow::onErrorOccurred);
    connect(socket, &QTcpSocket::readyRead, this, &MutiWindow::readServer);

    socket->connectToHost(ip, port);
}

void MutiWindow::readServer() {
    qDebug() << "Reading from server...";
    QByteArray data = socket->readAll();
    QString receivedMsg = QString::fromUtf8(data.trimmed());

    // 处理可能接收到的多条消息
    QStringList messages = receivedMsg.split('\n', Qt::SkipEmptyParts);
    for (const QString &msg : messages) {
        qDebug() << "Client received message:" << msg;

        if (msg == "Black" || msg == "White") {
            setPlayerColor(msg);
            resetBoard();
            update();
            continue; // 处理下一条消息
        }

        if (msg == "OpponentConnected") {
            connectionStatus->setText(QString("您是 ") + ((playerColor == 1) ? "Black" : "White"));
            isPlayerTurn = (playerColor == 1); // 黑色先开始
        } else if (msg == "OpponentLeft") {
            QMessageBox::information(this, tr("游戏结束"), tr("对方已退出游戏，你已获胜"));
            resetBoard(); // 重置棋盘
            socket->disconnectFromHost();
        } else if (msg == "UndoRequest") {
            int ret = QMessageBox::question(this, tr("悔棋请求"), tr("对方请求悔棋，是否同意？"),
                                            QMessageBox::Yes | QMessageBox::No);
            sendUndoResponse(ret == QMessageBox::Yes);
            if (ret == QMessageBox::Yes && !moveHistory.isEmpty() && !flipHistory.isEmpty()) {
                // 如果同意悔棋，立即撤回上一步棋
                QPair<QPoint, int> lastMove = moveHistory.pop();
                QVector<QPoint> lastFlipped = flipHistory.pop();
                int x = lastMove.first.x();
                int y = lastMove.first.y();
                int color = lastMove.second;

                // 恢复棋盘状态
                map[x][y] = 0;
                for (auto &p : lastFlipped) {
                    map[p.x()][p.y()] = 3 - color;  // 恢复为对手颜色
                }

                // 更新最后落子位置
                if (!moveHistory.isEmpty()) {
                    QPair<QPoint, int> prevMove = moveHistory.top();  // 获取历史中的最后一步
                    lastMoveRow = prevMove.first.y();
                    lastMoveCol = prevMove.first.x();
                } else {
                    lastMoveRow = -1;
                    lastMoveCol = -1;
                }
                update();
                isPlayerTurn = (playerColor == color); // 保持悔棋玩家的回合
            }
        } else if (msg == "UndoAccepted") {
            QMessageBox::information(this, tr("悔棋请求"), tr("对方同意悔棋请求"));
            undoAccepted();
        } else if (msg == "UndoRejected") {
            QMessageBox::information(this, tr("悔棋请求"), tr("对方拒绝悔棋请求"));
        } else if (msg == "DrawRequest") {
            int ret = QMessageBox::question(this, tr("求和请求"), tr("对方请求求和，是否同意？"),
                                            QMessageBox::Yes | QMessageBox::No);
            sendDrawResponse(ret == QMessageBox::Yes);
            if (ret == QMessageBox::Yes) {
                handleDrawAccepted();
            }
        } else if (msg == "DrawAccepted") {
            handleDrawAccepted();
        } else if (msg == "DrawRejected") {
            QMessageBox::information(this, tr("求和请求"), tr("对方拒绝求和请求"));
        } else if (msg == "Surrender") {
            QMessageBox::information(this, tr("游戏结束"), tr("对方认输，你已获胜"));
            resetBoard();
            update();
        } else if (msg == "SwapColorRequest") {
            int ret = QMessageBox::question(this, tr("交换颜色请求"), tr("对方请求交换颜色，是否同意？"),
                                            QMessageBox::Yes | QMessageBox::No);
            sendSwapColorResponse(ret == QMessageBox::Yes);
        } else if (msg == "SwapColorAccepted") {
            QMessageBox::information(this, tr("交换颜色请求"), tr("对方同意交换颜色请求"));
            setPlayerColor(playerColor == 1 ? "White" : "Black");
            resetBoard();  // 交换颜色后重置棋盘
            update();
        } else if (msg == "SwapColorRejected") {
            QMessageBox::information(this, tr("交换颜色请求"), tr("对方拒绝交换颜色请求"));
        } else if (msg.startsWith("CHAT:")) { // 处理聊天消息
            QString chatMsg = msg.mid(5);
            chatBox->append("对方: " + chatMsg);
        } else {
            QStringList parts = msg.split(",");
            if (parts.size() == 3) {
                int x = parts[0].toInt();
                int y = parts[1].toInt();
                int color = parts[2].toInt();
                processMove(x, y, color);
                if (isGameOver()) { // 检查游戏是否结束
                    calculateWinner();
                }
            }
        }
    }
}

void MutiWindow::undoAccepted() {
    if (!moveHistory.isEmpty() && !flipHistory.isEmpty()) {
        // 撤销上一步棋
        QPair<QPoint, int> lastMove = moveHistory.pop();
        QVector<QPoint> lastFlipped = flipHistory.pop();
        int x = lastMove.first.x();
        int y = lastMove.first.y();
        int color = lastMove.second;

        // 恢复棋盘状态
        map[x][y] = 0; // 撤销最后一步棋的位置
        for (auto &p : lastFlipped) {
            map[p.x()][p.y()] = 3 - color;  // 恢复棋子为对手的颜色
        }

        // 更新最后落子位置
        if (!moveHistory.isEmpty()) {
            QPair<QPoint, int> prevMove = moveHistory.top();  // 获取历史中的最后一步
            lastMoveRow = prevMove.first.y();
            lastMoveCol = prevMove.first.x();
        } else {
            lastMoveRow = -1;
            lastMoveCol = -1;
        }
        update(); // 重新绘制棋盘
        isPlayerTurn = (playerColor == color);  // 恢复悔棋玩家的回合
        QMessageBox::information(this, tr("悔棋请求"), tr("对方同意悔棋请求，已撤销上一步棋。"));
    } else {
        QMessageBox::information(this, tr("悔棋请求"), tr("没有可撤销的棋步。"));
    }
}

void MutiWindow::handleDrawAccepted() {
    QMessageBox::information(this, tr("求和请求"), tr("双方达成和局"));
    resetBoard();
}

void MutiWindow::handleSwapColorRequest() {
    if (!socket->isOpen()) {
        qDebug() << "Socket is not open";
        return;
    }
    qDebug() << "Sending swap color request";
    socket->write("SwapColorRequest\n");
    QMessageBox::information(this, tr("交换颜色请求"), tr("已发送交换颜色请求，等待对方响应"));
}


void MutiWindow::sendSwapColorResponse(bool accepted) {
    if (!socket->isOpen()) {
        qDebug() << "Socket is not open";
        return;
    }
    QString response = accepted ? "SwapColorAccepted\n" : "SwapColorRejected\n";
    socket->write(response.toUtf8());
    qDebug() << "Sending swap color response:" << response;
    if (accepted) {
        setPlayerColor(playerColor == 1 ? "White" : "Black");
        resetBoard();
        update();
    }
}

void MutiWindow::setPlayerColor(const QString &color) {
    if (color == "Black" || color == "White") {
        playerColor = (color == "Black") ? 1 : 2;
        hasBlackPlayer = (color == "Black");
        hasWhitePlayer = (color == "White");
        colorLabel->setText("颜色: " + color);
        connectionStatus->setText("连接成功，您是 " + color);
        isPlayerTurn = (color == "Black");
        qDebug() << "设置玩家颜色：" << color << " 当前玩家颜色：" << playerColor;
    } else {
        qDebug() << "收到未知颜色：" << color;
    }
}


void MutiWindow::resetBoard() {
    qDebug() << "Resetting board...";
    memset(map, 0, sizeof(map));
    map[3][3] = 1;
    map[4][4] = 1;
    map[3][4] = 2;
    map[4][3] = 2;
    row = -1;
    col = -1;
    currentX = -1;
    currentY = -1;
    lastMoveRow = -1;
    lastMoveCol = -1;
    moveHistory.clear(); // 清空棋步历史
    flipHistory.clear(); // 清空翻转历史

    isPlayerTurn = (playerColor == 1); // 黑色先开始

    qDebug() << "Board reset complete.";
    update();
}

void MutiWindow::mouseMoveEvent(QMouseEvent *event) {
    int x = (event->x() - xOffset) / cellSize;
    int y = (event->y() - yOffset) / cellSize;
    if (x >= 0 && x < borad_size && y >= 0 && y < borad_size) {
        if (currentX != x || currentY != y) {
            currentX = x;
            currentY = y;
            update();
        }
    } else {
        if (currentX != -1 || currentY != -1) {
            currentX = -1;
            currentY = -1;
            update();
        }
    }
}

void MutiWindow::mousePressEvent(QMouseEvent *event) {
    if (!isPlayerTurn) return;
    int x = (event->x() - xOffset) / cellSize;
    int y = (event->y() - yOffset) / cellSize;
    if (x >= 0 && x < borad_size && y >= 0 && y < borad_size && map[x][y] == 0) {
        if (isValidMove(x, y, playerColor)) {
            QVector<QPoint> flipped;
            map[x][y] = playerColor;
            executeFlip(x, y, playerColor, flipped);
            sendMove(x, y);
            update();
            lastMoveRow = y;
            lastMoveCol = x;
            moveHistory.push(qMakePair(QPoint(x, y), playerColor)); // 记录当前棋步
            flipHistory.push(flipped); // 记录当前翻转的棋子
            if (!hasValidMove(3 - playerColor)) { // 检查对手是否有合法移动
                isPlayerTurn = true; // 对手无合法移动，保持当前轮次
                QMessageBox::information(this, tr("提示"), tr("对手无合法移动，轮次保持"));
            } else {
                isPlayerTurn = false; // 对手有合法移动，切换轮次
            }
        }
    }
}

void MutiWindow::paintEvent(QPaintEvent *event) {
    QPainter pt(this);
    paintBoard(&pt);
    paintSquare(&pt);
    paintChess(&pt);

    // 更新棋子数量和回合标签
    int blackCount = 0;
    int whiteCount = 0;
    for (int i = 0; i < borad_size; ++i) {
        for (int j = 0; j < borad_size; ++j) {
            if (map[i][j] == 1) blackCount++;
            else if (map[i][j] == 2) whiteCount++;
        }
    }
    blackCountLabel->setText("黑棋数量: " + QString::number(blackCount));
    whiteCountLabel->setText("白棋数量: " + QString::number(whiteCount));
    turnLabel->setText("当前回合: " + QString((isPlayerTurn ? "你的" : "对手的")));
}

void MutiWindow::paintBoard(QPainter *pt) {
    QColor deepOak(150, 100, 60);
    QColor lightOak(190, 150, 90);

    pt->fillRect(xOffset - 5, yOffset - 5, boardWidth + 10, 5, Qt::darkGray);
    pt->fillRect(xOffset - 5, yOffset + boardHeight, boardWidth + 10, 5, Qt::darkGray);
    pt->fillRect(xOffset - 5, yOffset, 5, boardHeight, Qt::darkGray);
    pt->fillRect(xOffset + boardWidth, yOffset, 5, boardHeight, Qt::darkGray);

    pt->setPen(QPen(Qt::black, 2));
    for (int i = 0; i <= borad_size; ++i) {
        pt->drawLine(xOffset, yOffset + i * cellSize, xOffset + boardWidth, yOffset + i * cellSize);
    }
    for (int j = 0; j <= borad_size; ++j) {
        pt->drawLine(xOffset + j * cellSize, yOffset, xOffset + j * cellSize, yOffset + boardHeight);
    }

    for (int i = 0; i < borad_size; ++i) {
        for (int j = 0; j < borad_size; ++j) {
            pt->fillRect(xOffset + i * cellSize, yOffset + j * cellSize, cellSize, cellSize,
                         (i + j) % 2 == 0 ? deepOak : lightOak);
        }
    }
}

void MutiWindow::paintSquare(QPainter *pt) {
    if (currentX != -1 && currentY != -1) {
        pt->setPen(QPen(Qt::red, 2));
        pt->drawRect(xOffset + currentX * cellSize, yOffset + currentY * cellSize, cellSize, cellSize);
    }
}

void MutiWindow::paintChess(QPainter *pt) {
    for (int i = 0; i < borad_size; ++i) {
        for (int j = 0; j < borad_size; ++j) {
            if (map[i][j] != 0) {
                pt->setPen(Qt::black);
                pt->setBrush(QBrush(map[i][j] == 2 ? Qt::white : Qt::black, Qt::SolidPattern));
                pt->drawEllipse(xOffset + i * cellSize + (cellSize - pieceSize) / 2,
                                yOffset + j * cellSize + (cellSize - pieceSize) / 2,
                                pieceSize, pieceSize);

                if (i == lastMoveCol && j == lastMoveRow) {
                    pt->setBrush(Qt::red);
                    int dotSize = 10;
                    pt->drawEllipse(xOffset + i * cellSize + (cellSize - pieceSize) / 2 + (pieceSize - dotSize) / 2,
                                    yOffset + j * cellSize + (cellSize - pieceSize) / 2 + (pieceSize - dotSize) / 2,
                                    dotSize, dotSize);
                }
            } else if (isPlayerTurn && isValidMove(i, j, playerColor)) { // 仅在当前玩家回合时显示绿色提示点
                pt->setPen(Qt::NoPen);
                pt->setBrush(Qt::green);
                int hintSize = 20;
                pt->drawEllipse(xOffset + i * cellSize + (cellSize - hintSize) / 2,
                                yOffset + j * cellSize + (cellSize - hintSize) / 2,
                                hintSize, hintSize);
            }
        }
    }
}

bool MutiWindow::isValidMove(int x, int y, int color) {
    if (x < 0 || x >= borad_size || y < 0 || y >= borad_size || map[x][y] != 0) return false;

    bool valid = false;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            bool foundOpponent = false;
            while (nx >= 0 && nx < borad_size && ny >= 0 && ny < borad_size && map[nx][ny] == 3 - color) {
                foundOpponent = true;
                nx += dx;
                ny += dy;
            }
            if (foundOpponent && nx >= 0 && nx < borad_size && ny >= 0 && ny < borad_size && map[nx][ny] == color) {
                valid = true;
                break;
            }
        }
        if (valid) break;
    }
    return valid;
}

bool MutiWindow::hasValidMove(int color) {
    for (int i = 0; i < borad_size; ++i) {
        for (int j = 0; j < borad_size; ++j) {
            if (isValidMove(i, j, color)) {
                return true;
            }
        }
    }
    return false;
}

void MutiWindow::eat(int x, int y, int color, int dx, int dy, QVector<QPoint> &flipped) {
    int nx = x + dx;
    int ny = y + dy;
    QVector<QPoint> toFlip;
    while (nx >= 0 && nx < borad_size && ny >= 0 && ny < borad_size && map[nx][ny] == 3 - color) {
        toFlip.append(QPoint(nx, ny));
        nx += dx;
        ny += dy;
    }
    if (nx >= 0 && nx < borad_size && ny >= 0 && ny < borad_size && map[nx][ny] == color) {
        for (auto &p : toFlip) {
            map[p.x()][p.y()] = color;
            flipped.append(p);
        }
    }
}

void MutiWindow::executeFlip(int x, int y, int color, QVector<QPoint> &flipped) {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            eat(x, y, color, dx, dy, flipped);
        }
    }
}

bool MutiWindow::isBoardFull() {
    for (int i = 0; i < borad_size; ++i) {
        for (int j = 0; j < borad_size; ++j) {
            if (map[i][j] == 0) return false;
        }
    }
    return true;
}

bool MutiWindow::isGameOver() {
    if (isBoardFull()) return true;
    // 如果双方都无法落子，游戏结束
    if (!hasValidMove(1) && !hasValidMove(2)) {
        return true;
    }
    return false;
}

void MutiWindow::calculateWinner() {
    int blackCount = 0;
    int whiteCount = 0;
    for (int i = 0; i < borad_size; ++i) {
        for (int j = 0; j < borad_size; ++j) {
            if (map[i][j] == 1) blackCount++;
            else if (map[i][j] == 2) whiteCount++;
        }
    }
    QString result;
    if (blackCount > whiteCount) result = tr("黑色获胜！");
    else if (whiteCount > blackCount) result = tr("白色获胜！");
    else result = tr("平局！");
    QMessageBox::information(this, tr("游戏结束"), result);
    resetBoard();
}

void MutiWindow::handleUndoRequest() {
    if (!socket->isOpen()) {
        qDebug() << "Socket is not open";
        return;
    }
    qDebug() << "Sending undo request";
    socket->write("UndoRequest\n");
    QMessageBox::information(this, tr("悔棋请求"), tr("已发送悔棋请求，等待对方响应"));
}

void MutiWindow::sendUndoResponse(bool accepted) {
    if (!socket->isOpen()) {
        qDebug() << "Socket is not open";
        return;
    }
    QString response = accepted ? "UndoAccepted\n" : "UndoRejected\n";
    socket->write(response.toUtf8());
    qDebug() << "Sending undo response:" << response;
}

void MutiWindow::handleDrawRequest() {
    if (!socket->isOpen()) {
        qDebug() << "Socket is not open";
        return;
    }
    qDebug() << "Sending draw request";
    socket->write("DrawRequest\n");
    QMessageBox::information(this, tr("求和请求"), tr("已发送求和请求，等待对方响应"));
}

void MutiWindow::sendDrawResponse(bool accepted) {
    if (!socket->isOpen()) {
        qDebug() << "Socket is not open";
        return;
    }
    QString response = accepted ? "DrawAccepted\n" : "DrawRejected\n";
    socket->write(response.toUtf8());
    qDebug() << "Sending draw response:" << response;
    if (accepted) {
        handleDrawAccepted();
    }
}

void MutiWindow::handleSurrender() {
    if (!socket->isOpen()) {
        qDebug() << "Socket is not open";
        return;
    }
    qDebug() << "Sending surrender message";
    socket->write("Surrender\n");
    QMessageBox::information(this, tr("游戏结束"), tr("您已认输"));
    resetBoard();
}

void MutiWindow::handleReturnToMain() {
    if (socket->isOpen()) {
        socket->write("OpponentLeft\n");
        socket->disconnectFromHost();
    }

    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
}

void MutiWindow::processMove(int x, int y, int color) {
    // 检查该位置的移动是否有效
    if (isValidMove(x, y, color)) {
        // 将该位置设置为指定颜色
        map[x][y] = color;
        // 存储被翻转的棋子位置
        QVector<QPoint> flipped;
        // 执行翻转操作
        executeFlip(x, y, color, flipped);
        // 更新最后一个移动的行和列
        lastMoveRow = y;
        lastMoveCol = x;
        // 将当前移动记录到移动历史中
        moveHistory.push(qMakePair(QPoint(x, y), color));
        // 将翻转的棋子记录到翻转历史中
        flipHistory.push(flipped);
        // 更新棋盘显示
        update();
        // 切换轮次
        isPlayerTurn = !isPlayerTurn;
    }
}


void MutiWindow::sendMove(int x, int y) {
    if (!socket->isOpen()) return;
    QString move = QString::number(x) + "," + QString::number(y) + "," + QString::number(playerColor) + "\n";
    socket->write(move.toUtf8());
    isPlayerTurn = false; // 发送移动后轮次交给对方
    if (isGameOver()) { // 检查游戏是否结束
        calculateWinner();
    }
}

void MutiWindow::sendMessage() {
    if (!socket->isOpen()) {
        qDebug() << "Socket is not open";
        return;
    }
    QString message = chatInput->text();
    if (!message.isEmpty()) {
        chatBox->append("我: " + message);
        socket->write("CHAT:" + message.toUtf8() + "\n");
        chatInput->clear();
    }
}


MutiWindow::~MutiWindow() {
    delete ui;
}
