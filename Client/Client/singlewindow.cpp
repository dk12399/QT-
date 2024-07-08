#include "singlewindow.h"
#include "ui_singlewindow.h"
#include "mainwindow.h"
SingleWindow::SingleWindow(QWidget *parent) : QWidget(parent), ui(new Ui::SingleWindow) {
    ui->setupUi(this);
    setWindowTitle("黑白棋客户端-单人对战模式"); // 设置窗口标题
    setFixedSize(920, 680); // 改变窗口大小以适应新布局
    xOffset = 20; // 左侧偏移
    yOffset = 20; // 垂直居中
    this->setAutoFillBackground(true);
    this->setMouseTracking(true);
    resetBoard();
    setupUI();
    updateStatistics(); // 确保在棋盘设置后调用
}
void SingleWindow::setupUI() {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);  // 主布局

    QWidget *boardArea = new QWidget(this);
    boardArea->setMouseTracking(true);
    boardArea->setFixedSize(640, 800);  // 棋盘区域大小
    mainLayout->addWidget(boardArea);

    QVBoxLayout *rightPanelLayout = new QVBoxLayout();
    rightPanelLayout->setSpacing(10);
    rightPanelLayout->setAlignment(Qt::AlignTop);

    // 游戏设置区域
    QGroupBox *settingsGroup = new QGroupBox(tr("游戏设置"));
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsGroup);

    // 难度选择
    QHBoxLayout *difficultyLayout = new QHBoxLayout();
    QLabel *difficultyLabel = new QLabel(tr("当前难度是:"), this);
    difficultyLayout->addWidget(difficultyLabel);

    currentDifficultyLabel = new QLabel(tr("简易"), this);
    difficultyLayout->addWidget(currentDifficultyLabel);

    QComboBox *aiSwitch = new QComboBox(this);
    aiSwitch->addItems({tr("简易"), tr("中等"), tr("困难")});
    aiSwitch->setMaximumWidth(200);
    difficultyLayout->addWidget(aiSwitch);
   connect(aiSwitch, QOverload<int>::of(&QComboBox::activated), this, &SingleWindow::changeAiMode);

    // 颜色选择
    QHBoxLayout *colorLayout = new QHBoxLayout();
    QLabel *currentColorLabel = new QLabel(tr("你的颜色是:"), this);
    colorLayout->addWidget(currentColorLabel);

    QLabel *playerColorLabel = new QLabel(tr("黑子"), this);
    playerColorLabel->setObjectName("playerColorLabel");
    colorLayout->addWidget(playerColorLabel);

    QPushButton *colorChangeButton = new QPushButton(tr("切换颜色"), this);
    colorLayout->addWidget(colorChangeButton);
    connect(colorChangeButton, &QPushButton::clicked, this, &SingleWindow::changeColor);

    settingsLayout->addLayout(difficultyLayout);
    settingsLayout->addLayout(colorLayout);
    rightPanelLayout->addWidget(settingsGroup);

    // 棋局统计信息区
    QGroupBox *statsGroup = new QGroupBox(tr("游戏统计"));
    QVBoxLayout *statsLayout = new QVBoxLayout(statsGroup);

    QLabel *blackCountLabel = new QLabel(tr("黑子数目：0"), this);
    blackCountLabel->setObjectName("blackCountLabel");
    blackCountLabel->setFont(QFont("Arial", 12));
    statsLayout->addWidget(blackCountLabel);

    QLabel *whiteCountLabel = new QLabel(tr("白子数目：0"), this);
    whiteCountLabel->setObjectName("whiteCountLabel");
    whiteCountLabel->setFont(QFont("Arial", 12));
    statsLayout->addWidget(whiteCountLabel);

    QLabel *roundCountLabel = new QLabel(tr("回合数：1"), this);
    roundCountLabel->setObjectName("roundCountLabel");
    roundCountLabel->setFont(QFont("Arial", 12));
    statsLayout->addWidget(roundCountLabel);

    rightPanelLayout->addWidget(statsGroup);

    // 控制按钮区域
    QGroupBox *controlButtonsGroup = new QGroupBox(tr("游戏控制"));
    QVBoxLayout *buttonsLayout = new QVBoxLayout(controlButtonsGroup);

    QPushButton *undoButton = new QPushButton(tr("悔棋"), this);
    undoButton->setFont(QFont("Arial", 14));
    buttonsLayout->addWidget(undoButton);
    connect(undoButton, &QPushButton::clicked, this, &SingleWindow::undoMove);

    QPushButton *backButton = new QPushButton(tr("返回主页面"), this);
    backButton->setFont(QFont("Arial", 14));
    buttonsLayout->addWidget(backButton);
    connect(backButton, &QPushButton::clicked, this, &SingleWindow::returnToMain);

    rightPanelLayout->addWidget(controlButtonsGroup);

    // 棋局分析图表区域
    // QGroupBox *chartGroup = new QGroupBox(tr("棋局分析"));
    // QVBoxLayout *chartLayout = new QVBoxLayout(chartGroup);

    // QChart *chart = new QChart();
    // QLineSeries *seriesBlack = new QLineSeries();
    // seriesBlack->append(0, 2); // 初始数据
    // seriesBlack->setName(tr("黑子"));
    // QLineSeries *seriesWhite = new QLineSeries();
    // seriesWhite->append(0, 2); // 初始数据
    // seriesWhite->setName(tr("白子"));

    // chart->addSeries(seriesBlack);
    // chart->addSeries(seriesWhite);
    // chart->createDefaultAxes();
    // chart->setTitle(tr("棋子数目变化"));

    // QChartView *chartView = new QChartView(chart);
    // chartView->setRenderHint(QPainter::Antialiasing);
    // chartView->setMinimumHeight(250);  // 设置图表的最小高度
    // chartView->setMaximumHeight(300);  // 设置图表的最大高度以避免过大
    // chartLayout->addWidget(chartView);

    // rightPanelLayout->addWidget(chartGroup);

    QWidget *rightPanel = new QWidget();
    rightPanel->setLayout(rightPanelLayout);
    mainLayout->addWidget(rightPanel);

    // 调整右侧面板的宽度
    rightPanel->setMinimumWidth(240);
    rightPanel->setMaximumWidth(300);  // 根据需要调整最大宽度

    this->setLayout(mainLayout);
}
void SingleWindow::updateStatistics() {
    int blackCount = 0;
    int whiteCount = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (map[i][j] == 1) ++blackCount;
            else if (map[i][j] == 2) ++whiteCount;
        }
    }

    QLabel *blackLabel = findChild<QLabel*>("blackCountLabel");
    blackLabel->setText(tr("黑子数目：") + QString::number(blackCount));

    QLabel *whiteLabel = findChild<QLabel*>("whiteCountLabel");
    whiteLabel->setText(tr("白子数目：") + QString::number(whiteCount));

    QLabel *roundLabel = findChild<QLabel*>("roundCountLabel");
    int currentRound = historyStack.size() + 1; // 假设每次向 historyStack 推送表示一个回合
    roundLabel->setText(tr("回合数：") + QString::number(currentRound));
}

void SingleWindow::returnToMain() {
    this->close();  // 关闭当前窗口

    MainWindow *mainWindow = new MainWindow(); // 创建主窗口实例
    mainWindow->show();  // 显示主窗口
}

void SingleWindow::changeColor() {
    playerColor = (playerColor == 1) ? 2 : 1;  // 切换玩家颜色
    QLabel *playerColorLabel = this->findChild<QLabel*>("playerColorLabel");
    if (playerColorLabel) {
        playerColorLabel->setText(playerColor == 2 ? tr("White") : tr("Black"));  // 更新按钮显示
    }
    resetBoard();  // 重置棋盘
    updateStatistics();  // 更新统计信息
    // 如果玩家选择白子，AI 作为黑子先行
    if (playerColor == 2) {
        isPlayerTurn = false;  // 设置为 AI 的回合
        aiMove(ai);  // 调用 AI 下棋函数
    } else {
        isPlayerTurn = true;  // 确保玩家回合正确设置
    }
}



void SingleWindow::resetBoard() {
    memset(map, 0, sizeof(map)); // 重置棋盘
    map[3][3] = 1; // 初始化棋盘中心的四个棋子
    map[4][4] = 1;
    map[3][4] = 2;
    map[4][3] = 2;

    players = 0; // 重置玩家和AI的棋子计数
    AIs = 0;
    row = -1; // 重置最后一步的行列索引
    col = -1;
    isPlayerTurn = true; // 重置为玩家回合
    currentX = -1; // 重置当前鼠标停留的格子位置
    currentY = -1;
    historyStack.clear(); // 清空历史记录堆栈
    lastMoveRow = -1; // 重置最后落子的行位置
    lastMoveCol = -1; // 重置最后落子的列位置
    update(); // 请求更新窗口显示
}


void SingleWindow::undoMove() {
    if (!historyStack.isEmpty()) {
        // 恢复到上一次的棋盘状态
        QVector<QVector<int>> lastState = historyStack.pop();
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                map[i][j] = lastState[i][j];
            }
        }

        // 恢复上一步的落子位置
        if (!historyStack.isEmpty()) {
            QVector<QVector<int>> previousState = historyStack.top();
            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    if (previousState[i][j] != map[i][j]) {
                        lastMoveRow = j;
                        lastMoveCol = i;
                    }
                }
            }
        } else {
            // 如果历史栈为空，则重置最后的落子位置
            lastMoveRow = -1;
            lastMoveCol = -1;
        }

        update();
        isPlayerTurn = isPlayerTurn?isPlayerTurn:!isPlayerTurn; // 切换回合
        updateStatistics();  // 更新统计信息
    }
}


void SingleWindow::changeAiMode(int index) {
    QString difficulty;
    switch (index) {
    case 0:
        difficulty = tr("简易");
        ai = EASY;
        break;
    case 1:
        difficulty = tr("中等");
        ai = MEDIUM;
        break;
    case 2:
        difficulty = tr("困难");
        ai = HARD;
        break;
    default:
        difficulty = tr("未知");
        ai = EASY; // Default to EASY or handle error
    }
    currentDifficultyLabel->setText(difficulty);
    qDebug() << "Difficulty changed to" << difficulty;
    resetBoard();
    updateStatistics();
}



void SingleWindow::mouseMoveEvent(QMouseEvent *event) {
    int x = (event->x() - xOffset) / cellSize;
    int y = (event->y() - yOffset) / cellSize;



    // 检查是否在棋盘内，并且坐标与之前的坐标不同
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
        if (currentX != x || currentY != y) {
            currentX = x;
            currentY = y;
            qDebug() << "Updating due to move inside board"; // 调试输出更新原因
            update();  // 触发更新
        }
    } else {
        if (currentX != -1 || currentY != -1) {
            currentX = -1;
            currentY = -1;
            qDebug() << "Updating due to move outside board"; // 调试输出更新原因
            update();  // 触发更新
        }
    }
}




void SingleWindow::mousePressEvent(QMouseEvent *event) {
    if (!isPlayerTurn) return;
    int x = (event->x() - xOffset) / cellSize;
    int y = (event->y() - yOffset) / cellSize;
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && map[x][y] == 0) {
        if (isValidMove(x, y, playerColor)) {
            QVector<QVector<int>> currentState(BOARD_SIZE, QVector<int>(BOARD_SIZE));
            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    currentState[i][j] = map[i][j];
                }
            }
            historyStack.push(currentState);
            map[x][y] = playerColor;
            executeFlip(x, y, playerColor);
            update();
            lastMoveRow = y; // 更新最后落子位置
            lastMoveCol = x;
            isPlayerTurn = false;
            updateStatistics();  // 更新统计信息
            checkGameStateAndProceed();
        }
    }
}

void SingleWindow::paintEvent(QPaintEvent *event) {
    QPainter pt(this);
    paintBoard(&pt);
    paintSquare(&pt);
    paintChess(&pt);
}

void SingleWindow::paintBoard(QPainter *pt) {
    QColor deepOak(150, 100, 60);
    QColor lightOak(190, 150, 90);

    pt->fillRect(xOffset - 5, yOffset - 5, boardWidth + 10, 5, Qt::darkGray);
    pt->fillRect(xOffset - 5, yOffset + boardHeight, boardWidth + 10, 5, Qt::darkGray);
    pt->fillRect(xOffset - 5, yOffset, 5, boardHeight, Qt::darkGray);
    pt->fillRect(xOffset + boardWidth, yOffset, 5, boardHeight, Qt::darkGray);

    pt->setPen(QPen(Qt::black, 2));
    for (int i = 0; i <= BOARD_SIZE; ++i) {
        pt->drawLine(xOffset, yOffset + i * cellSize, xOffset + boardWidth, yOffset + i * cellSize);
    }
    for (int j = 0; j <= BOARD_SIZE; ++j) {
        pt->drawLine(xOffset + j * cellSize, yOffset, xOffset + j * cellSize, yOffset + boardHeight);
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            pt->fillRect(xOffset + i * cellSize, yOffset + j * cellSize, cellSize, cellSize,
                         (i + j) % 2 == 0 ? deepOak : lightOak);
        }
    }
}

void SingleWindow::paintSquare(QPainter *pt) {
    if (currentX != -1 && currentY != -1) {
        pt->setPen(QPen(Qt::red, 2));  // 使用更粗的笔画来确保外框可见
        pt->drawRect(xOffset + currentX * cellSize, yOffset + currentY * cellSize, cellSize, cellSize);
    }
}


void SingleWindow::paintChess(QPainter *pt) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (map[i][j] != 0) {
                // 绘制棋子
                pt->setPen(Qt::black);
                pt->setBrush(QBrush(map[i][j] == 2 ? Qt::white : Qt::black, Qt::SolidPattern));
                pt->drawEllipse(xOffset + i * cellSize + (cellSize - pieceSize) / 2,
                                yOffset + j * cellSize + (cellSize - pieceSize) / 2,
                                pieceSize, pieceSize);

                // 如果当前位置是最后落子的位置，绘制红色小点
                if (i == lastMoveCol && j == lastMoveRow) {
                    pt->setBrush(Qt::red);
                    int dotSize = 10; // 小点的直径
                    pt->drawEllipse(xOffset + i * cellSize + (cellSize - pieceSize) / 2 + (pieceSize - dotSize) / 2,
                                    yOffset + j * cellSize + (cellSize - pieceSize) / 2 + (pieceSize - dotSize) / 2,
                                    dotSize, dotSize);
                }
            } else if (isValidMove(i, j, playerColor)) {
                // 绘制绿色合法落子提示
                pt->setPen(Qt::NoPen); // 不使用边框
                pt->setBrush(Qt::green);
                int hintSize = 20; // 提示点的直径
                pt->drawEllipse(xOffset + i * cellSize + (cellSize - hintSize) / 2,
                                yOffset + j * cellSize + (cellSize - hintSize) / 2,
                                hintSize, hintSize);
            }
        }
    }
}




bool SingleWindow::isValidMove(int x, int y, int color) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || map[x][y] != 0) return false;

    bool valid = false;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            bool foundOpponent = false;
            while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && map[nx][ny] == 3 - color) {
                foundOpponent = true;
                nx += dx;
                ny += dy;
            }
            if (foundOpponent && nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && map[nx][ny] == color) {
                valid = true;
                break;
            }
        }
        if (valid) break;
    }
    return valid;
}

void SingleWindow::eat(int x, int y, int color, int dx, int dy) {
    int nx = x + dx;
    int ny = y + dy;
    QVector<QPoint> toFlip;
    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && map[nx][ny] == 3 - color) {
        toFlip.append(QPoint(nx, ny));
        nx += dx;
        ny += dy;
    }
    if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && map[nx][ny] == color) {
        for (auto &p : toFlip) {
            map[p.x()][p.y()] = color;
        }
    }
}

void SingleWindow::executeFlip(int x, int y, int color) {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            eat(x, y, color, dx, dy);
        }
    }
}

void SingleWindow::findLegalMoves(int color, QVector<QPoint> &legalMoves) {
    legalMoves.clear();
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (isValidMove(i, j, color)) {
                legalMoves.append(QPoint(i, j));
            }
        }
    }
}

void SingleWindow::aiMove(AIMode mode) {
    switch(mode) {
    case EASY: aiMoveEasy(); break;
    case MEDIUM: aiMoveMedium(); break;
    case HARD: aiMoveHard(); break;
    }
    updateStatistics();  // 在 AI 移动后更新统计信息
}

void SingleWindow::aiMoveEasy() {
    selectBestMove(3 - playerColor, 4);
}

void SingleWindow::aiMoveMedium() {
    selectBestMove(3 - playerColor, 5);
}

void SingleWindow::aiMoveHard() {
    selectBestMove(3 - playerColor, 6);
}

bool SingleWindow::isBoardFull() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (map[i][j] == 0) return false;
        }
    }
    return true;
}

void SingleWindow::calculateWinner() {
    int blackCount = 0;
    int whiteCount = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (map[i][j] == 1) blackCount++;
            else if (map[i][j] == 2) whiteCount++;
        }
    }
    QString result;
    if (blackCount > whiteCount) result = tr("黑方获胜");
    else if (whiteCount > blackCount) result = tr("白方获胜!");
    else result = tr("平局!");
    QMessageBox::information(this, tr("游戏结束"), result);
    resetBoard();
}

void SingleWindow::checkGameStateAndProceed() {
    QVector<QPoint> playerLegalMoves;
    QVector<QPoint> aiLegalMoves;
    findLegalMoves(playerColor, playerLegalMoves);
    findLegalMoves(3 - playerColor, aiLegalMoves);
    if ((playerLegalMoves.isEmpty() && aiLegalMoves.isEmpty()) || isBoardFull()) {
        calculateWinner();
        return;
    }
    if (isPlayerTurn && playerLegalMoves.isEmpty()) {
        qDebug() << "No valid moves for player, skipping turn.";
        isPlayerTurn = false;
        aiMove(ai);
        return;
    }
    if (!isPlayerTurn && aiLegalMoves.isEmpty()) {
        qDebug() << "No valid moves for AI, skipping turn.";
        isPlayerTurn = true;
        return;
    }
    if (!isPlayerTurn) {
        aiMove(ai);
    }
}

void SingleWindow::saveBoardState(QVector<QVector<int>>& savedState) {
    savedState.resize(BOARD_SIZE);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        savedState[i].resize(BOARD_SIZE);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            savedState[i][j] = map[i][j];
        }
    }
}

void SingleWindow::restoreBoardState(const QVector<QVector<int>>& savedState) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            map[i][j] = savedState[i][j];
        }
    }
}

void SingleWindow::selectBestMove(int color, int depth) {
    QVector<QPoint> legalMoves;
    findLegalMoves(color, legalMoves);
    if (legalMoves.isEmpty()) {
        qDebug() << "No valid moves available for color" << color;
        return;
    }
    int bestScore = (color == playerColor) ? INT_MIN : INT_MAX;
    QPoint bestMove = legalMoves.first();
    for (const auto& move : legalMoves) {
        int score = evaluateMove(move, color, depth);
        if ((color == playerColor && score > bestScore) || (color != playerColor && score < bestScore)) {
            bestScore = score;
            bestMove = move;
        }
    }
    performMove(bestMove.x(), bestMove.y(), color);
}

int SingleWindow::evaluateMove(const QPoint& move, int color, int depth) {
    QVector<QVector<int>> savedState;
    saveBoardState(savedState);
    map[move.x()][move.y()] = color;
    executeFlip(move.x(), move.y(), color);
    int score = minimax(depth, 3 - color, INT_MIN, INT_MAX, color != playerColor);
    restoreBoardState(savedState);
    return score;
}

void SingleWindow::performMove(int x, int y, int color) {
    map[x][y] = color;
    executeFlip(x, y, color);
    lastMoveCol = x; // 更新最后落子列
    lastMoveRow = y; // 更新最后落子行
    update();
    isPlayerTurn = !isPlayerTurn;
    checkGameStateAndProceed();
}

int SingleWindow::evaluateBoard(int color) {
    int blackScore = 0, whiteScore = 0;
    int blackMobility = 0, whiteMobility = 0;
    QVector<QPoint> blackMoves, whiteMoves;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            int piece = map[i][j];
            int addScore = positionWeights[i][j];
            if (piece == 1) {
                blackScore += addScore;
                appendValidMoves(i, j, 1, blackMoves);
            } else if (piece == 2) {
                whiteScore += addScore;
                appendValidMoves(i, j, 2, whiteMoves);
            }
        }
    }
    blackMobility = blackMoves.size();
    whiteMobility = whiteMoves.size();//可移动子
    int mobilityScore = blackMobility - whiteMobility;
    int finalScore = (blackScore - whiteScore) + 10 * mobilityScore;
    int cornerValue = 25;//角落控制
    if (map[0][0] == color) finalScore += cornerValue;
    if (map[0][BOARD_SIZE - 1] == color) finalScore += cornerValue;
    if (map[BOARD_SIZE - 1][0] == color) finalScore += cornerValue;
    if (map[BOARD_SIZE - 1][BOARD_SIZE - 1] == color) finalScore += cornerValue;
    return color == 1 ? finalScore : -finalScore;
}

bool SingleWindow::isStable(int x, int y, int color) {
    static const int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};//八个方向
    for (const auto& dir : directions) {
        int nx = x + dir[0];
        int ny = y + dir[1];
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && map[nx][ny] != color) {
            return false;
        }
    }
    return true;
}

bool SingleWindow::isFrontier(int x, int y) {
    if (map[x][y] == 0) return false;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && map[nx][ny] == 0) return true;
        }
    }
    return false;
}

void SingleWindow::appendValidMoves(int x, int y, int color, QVector<QPoint>& moves) {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (isValidMove(x + dx, y + dy, color) && !moves.contains(QPoint(x + dx, y + dy))) moves.append(QPoint(x + dx, y + dy));
        }
    }
}

void SingleWindow::checkAndCleanHashTable() {
    const int MaxHashEntries = 10000;
    if (transpositionTable.size() > MaxHashEntries) {
        transpositionTable.clear();
    }
}
//maximizingPlayer true 最大化自己的分 false最小化玩家的分
int SingleWindow::minimax(int depth, int color, int alpha, int beta, bool maximizingPlayer) {
    // 生成当前棋盘状态的哈希值，用于查找和存储在置换表中
    quint64 hash = generateBoardHash();
    // 递归终止条件：深度为0或棋盘已满时，返回当前棋盘状态的评价分数
    if (depth == 0 || isBoardFull()) {
        return evaluateBoard(color);
    }
    // 检查置换表中是否包含当前棋盘状态的哈希值，如果存在则直接返回存储的评价分数
    if (transpositionTable.contains(hash)) {
        return transpositionTable[hash];
    }
    // 初始化最佳值
    int bestValue = maximizingPlayer ? INT_MIN : INT_MAX;
    // 生成当前棋盘状态下所有可能的合法移动
    QVector<QPoint> legalMoves;
    findLegalMoves(color, legalMoves);
    // 如果没有合法移动，返回当前棋盘状态的评价分数
    if (legalMoves.isEmpty()) {
        return evaluateBoard(color);
    }
    // 保存当前棋盘状态，以便在递归调用后恢复
    QVector<QVector<int>> savedState(BOARD_SIZE, QVector<int>(BOARD_SIZE));
    saveBoardState(savedState);
    // 遍历所有合法移动
    for (const auto& move : legalMoves) {
        // 执行移动
        map[move.x()][move.y()] = color;
        executeFlip(move.x(), move.y(), color);
        // 递归调用 minimax 函数，搜索更深层次的棋盘状态
        int value = minimax(depth - 1, 3 - color, alpha, beta, !maximizingPlayer);
        // 恢复棋盘状态
        restoreBoardState(savedState);
        // 更新最佳值
        if (maximizingPlayer) {
            bestValue = std::max(bestValue, value); // 最大化自己选择得分最大的移动
            alpha = std::max(alpha, value);
        } else {
            bestValue = std::min(bestValue, value);// 最小化玩家选择得分最小的移动
            beta = std::min(beta, value);
        }
        // Alpha-Beta 剪枝
        if (beta <= alpha) break;
    }
    // 将当前棋盘状态的哈希值和计算得到的最佳值存储在置换表中
    transpositionTable[hash] = bestValue;
    // 返回最佳值
    return bestValue;
}


quint64 SingleWindow::generateBoardHash() {
    quint64 hash = 0;
    quint64 prime = 31;//质数防止哈希值重复
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            hash = hash * prime + map[i][j];//将棋盘状态赋予哈希值
        }
    }
    return hash;
}

SingleWindow::~SingleWindow() {
    delete ui;
    delete aiSwitch;
    delete colorChangeButton;
    delete undoButton;
}
