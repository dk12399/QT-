// #include "singlewindow.h"
// #include "ui_singlewindow.h"
// #include<QTimer>
// #include<QMessageBox>
// #include<QVector>
// #include <QPen>
// #include <QGraphicsScene>
// #include <QGraphicsPixmapItem>
// #include <QGraphicsBlurEffect>
// #include <QtGlobal>    // qsrand和qrand这两个函数在这里面
// #include <QHash>
// // 构造函数

// SingleWindow::SingleWindow(QWidget *parent)
//     : QWidget(parent)
//     , ui(new Ui::SingleWindow)
// {
//     ui->setupUi(this);
//     this->setFixedSize(800,800); // 设置窗口固定大小
//     xOffset = (width() - boardWidth) / 2; // 计算X轴偏移
//     yOffset = (height() - boardHeight) / 2; // 计算Y轴偏移
//     this->setAutoFillBackground(true); // 自动根据颜色设置背景色
//     this->setMouseTracking(true); // 开启鼠标追踪
//     resetBoard(); // 重置棋盘
//     setupUI(); // 设置UI
// }

// void SingleWindow::setupUI(){
//     // 创建难度选择标签
//     QLabel *difficultyLabel = new QLabel("难度选择：", this);
//     difficultyLabel->setGeometry(100, yOffset - 50, 100, 30);

//     // 创建下拉框
//     AISwitch = new QComboBox(this);
//     AISwitch->addItem("简单");
//     AISwitch->addItem("中等");
//     AISwitch->addItem("困难");

//     // 设置下拉框位置和大小
//     AISwitch->setGeometry(160, yOffset - 50, 100, 30);
//     connect(AISwitch, QOverload<int>::of(&QComboBox::activated),
//             this, &SingleWindow::changedAiMode);

//     // 创建标签用于指示玩家棋子颜色
//     QLabel *playerColorLabel = new QLabel("您的棋子颜色：", this);
//     playerColorLabel->setGeometry(270, yOffset - 50, 100, 30);

//     // 创建按钮用于切换玩家颜色
//     ColorChanged = new QPushButton("黑子", this);
//     ColorChanged->setGeometry(350, yOffset - 50, 70, 30);
//     connect(ColorChanged, &QPushButton::clicked, this, &SingleWindow::changedColor);
// }

// // 切换玩家颜色的函数
// void SingleWindow::changedColor() {
//     if (playerColor == 1) {
//         playerColor = 2; // 切换为白色
//         ColorChanged->setText("白子"); // 更新按钮文本为白子
//     } else {
//         playerColor = 1; // 切换为黑色
//         ColorChanged->setText("黑子"); // 更新按钮文本为黑子
//     }
//     resetBoard(); // 切换颜色后重置棋盘
// }

// void SingleWindow::resetBoard() {
//     players = 0;
//     AIs = 0;
//     row = -1;
//     col = -1;
//     isPlayerTurn = true;
//     currentX = -1;
//     currentY = -1;
//     memset(map, 0, sx * sy * sizeof(int)); // 清空棋盘
//     map[3][3] = 1; // 设置初始黑棋位置
//     map[4][4] = 1;
//     map[3][4] = 2; // 设置初始白棋位置
//     map[4][3] = 2;
//     update(); // 更新显示
// }

// // 实现处理下拉框选择的槽函数
// void SingleWindow::changedAiMode(int index) {
//     ai = static_cast<AIMode>(index); // 根据下拉框的选择更新AI难度
//     resetBoard(); // 重置棋盘
// }

// void SingleWindow::mouseMoveEvent(QMouseEvent *event)
// {
//     int x = (event->x() - xOffset) / cellSize; // 计算鼠标点击的X坐标
//     int y = (event->y() - yOffset) / cellSize; // 计算鼠标点击的Y坐标

//     // 检查鼠标是否在有效范围内
//     if (x >= 0 && x < boardSize && y >= 0 && y < boardSize)
//     {
//         currentX = x; // 更新当前鼠标位置X
//         currentY = y; // 更新当前鼠标位置Y
//         update(); // 发出更新信号以重绘窗口
//     }
// }

// void SingleWindow::mousePressEvent(QMouseEvent *event)
// {
//     if (!isPlayerTurn) return; // 如果不是玩家回合，则不处理

//     int x = (event->x() - xOffset) / cellSize;
//     int y = (event->y() - yOffset) / cellSize;
//     if (x >= 0 && x < 8 && y >= 0 && y < 8 && map[x][y] == 0) // 确保点击在棋盘范围内且为有效位置
//     {
//         if (isValidMove(x, y, playerColor)) // 检查是否为合法移动
//         {
//             map[x][y] = playerColor; // 放置棋子
//             executeFlip(x, y, playerColor); // 执行翻转
//             update(); // 更新显示
//             isPlayerTurn = false; // 切换到AI回合
//             checkGameStateAndProceed(); // 检测游戏是否结束，没结束进行落子
//         }
//     }
// }

// void SingleWindow::paintEvent(QPaintEvent *event)
// {
//     QPainter pt(this); // 创建绘图对象
//     paintboard(&pt); // 绘制棋盘
//     paintSquare(&pt); // 绘制框框
//     paintChess(&pt); // 绘制棋子
// }

// void SingleWindow::paintboard(QPainter *pt){
//     // 绘制棋盘
//     // 定义深橡木颜色和浅橡木颜色
//     QColor deepOak(150, 100, 60); // 稍微暗一些的深橡木颜色
//     QColor lightOak(190, 150, 90); // 稍微暗一些的浅橡木颜色

//     // 绘制外框
//     pt->fillRect(xOffset - 5, yOffset - 5, boardWidth + 10, 5, Qt::darkGray); // 上方填充
//     pt->fillRect(xOffset - 5, yOffset + boardHeight, boardWidth + 10, 5, Qt::darkGray); // 下方填充
//     pt->fillRect(xOffset - 5, yOffset, 5, boardHeight, Qt::darkGray); // 左侧填充
//     pt->fillRect(xOffset + boardWidth, yOffset, 5, boardHeight, Qt::darkGray); // 右侧填充

//     // 绘制棋盘内部的黑色线条
//     pt->setPen(QPen(Qt::black, 2)); // 设置画笔为黑色，线条宽度为2
//     for(int i = 0; i <= boardSize; ++i) // 绘制横线
//     {
//         pt->drawLine(xOffset, yOffset + i * cellSize, xOffset + boardWidth, yOffset + i * cellSize);
//     }
//     for(int j = 0; j <= boardSize; ++j) // 绘制竖线
//     {
//         pt->drawLine(xOffset + j * cellSize, yOffset, xOffset + j * cellSize, yOffset + boardHeight);
//     }

//     // 绘制棋盘背景
//     for (int i = 0; i < boardSize; ++i) {
//         for (int j = 0; j < boardSize; ++j) {
//             pt->fillRect(xOffset + i * cellSize, yOffset + j * cellSize, cellSize, cellSize,
//                          (i + j) % 2 == 0 ? deepOak : lightOak); // 填充颜色
//         }
//     }
// }

// void SingleWindow::paintSquare(QPainter *pt){
//     // 绘制鼠标位置框线
//     if (currentX != -1 && currentY != -1) // 如果鼠标当前位置有效
//     {
//         pt->setPen(Qt::red); // 框框颜色
//         pt->drawRect(xOffset + currentX * cellSize, yOffset + currentY * cellSize, cellSize, cellSize); // 绘制框框
//     }
// }

// void SingleWindow::paintChess(QPainter *pt) {
//     for (int i = 0; i < boardSize; ++i) {
//         for (int j = 0; j < boardSize; ++j) {
//             if (map[i][j] != 0) {
//                 pt->setPen(Qt::black);
//                 pt->setBrush(QBrush(map[i][j] == 2 ? Qt::white : Qt::black, Qt::SolidPattern));
//                 pt->drawEllipse(xOffset + i * cellSize + (cellSize - pieceSize) / 2,
//                                 yOffset + j * cellSize + (cellSize - pieceSize) / 2,
//                                 pieceSize, pieceSize); // 绘制棋子
//             } else if (isValidMove(i, j, playerColor)) {
//                 pt->setPen(Qt::black);
//                 pt->setBrush(QBrush(Qt::green, Qt::SolidPattern));
//                 pt->drawEllipse(xOffset + i * cellSize + (cellSize - 20) / 2,
//                                 yOffset + j * cellSize + (cellSize - 20) / 2, 20, 20); // 绘制提示
//             }
//         }
//     }
// }

// bool SingleWindow::isValidMove(int x, int y, int color)
// {
//     // 检查落子位置是否为空
//     if (map[x][y] != 0) return false;
//     // 遍历八个方向，检查是否有可以翻转对方棋子的情况
//     for (int dx = -1; dx <= 1; ++dx) {
//         for (int dy = -1; dy <= 1; ++dy) {
//             if (dx == 0 && dy == 0) continue; // 跳过不合法的方向
//             int nx = x + dx;
//             int ny = y + dy;
//             bool foundOpponent = false; // 标记是否找到对方棋子
//             while (nx >= 0 && nx < sx && ny >= 0 && ny < sy && map[nx][ny] == 3 - color) {
//                 nx += dx;
//                 ny += dy;
//                 foundOpponent = true;
//             }
//             // 如果在搜索过程中找到同色棋子，则该方向上的落子是合法的
//             if (foundOpponent && nx >= 0 && nx < sx && ny >= 0 && ny < sy && map[nx][ny] == color && (nx != x + dx || ny != y + dy)) {
//                 return true;
//             }
//         }
//     }
//     return false; // 如果没有找到可以翻转对方棋子的情况，则落子是不合法的
// }

// void SingleWindow::eat(int x, int y, int color, int dx, int dy)
// {
//     int nx = x + dx;
//     int ny = y + dy;
//     QVector<QPoint> toFlip; // 用来存储需要翻转的棋子位置

//     // 遍历指定方向，收集所有需要翻转的棋子
//     while (nx >= 0 && nx < sx && ny >= 0 && ny < sy && map[nx][ny] == 3 - color) {
//         toFlip.append(QPoint(nx, ny));
//         nx += dx;
//         ny += dy;
//     }

//     // 确保最终位置是自己的棋子，才执行翻转
//     if (nx >= 0 && nx < sx && ny >= 0 && ny < sy && map[nx][ny] == color) {
//         for (auto &p : toFlip) {
//             map[p.x()][p.y()] = color;
//         }
//     }
// }

// void SingleWindow::executeFlip(int x, int y, int color)
// {
//     for (int dx = -1; dx <= 1; ++dx) {
//         for (int dy = -1; dy <= 1; ++dy) {
//             if (dx == 0 && dy == 0) continue; // 跳过当前位置
//             eat(x, y, color, dx, dy); // 沿当前方向执行吃子
//         }
//     }
// }

// void SingleWindow::findLegalMoves(int color, QVector<QPoint> &legalMoves) {
//     legalMoves.clear(); // 清空之前的合法移动

//     for (int i = 0; i < sx; ++i) {
//         for (int j = 0; j < sy; ++j) {
//             if (isValidMove(i, j, color)) {
//                 legalMoves.append(QPoint(i, j)); // 添加合法移动位置
//             }
//         }
//     }
// }

// void SingleWindow::AIMove(AIMode mode) {
//     switch(mode){
//     case EASY: AIMoveEasy(); break; // 调用Easy难度下的AI函数
//     case MEDIUM: AIMoveMedium(); break; // 中等AI
//     case HARD: AIMoveHard(); break; // 困难AI
//     }
// }

// void SingleWindow::AIMoveEasy() {
//     QVector<QPoint> legalMoves; // 存储所有合法的落子位置
//     findLegalMoves(3 - playerColor, legalMoves); // 遍历棋盘，找到所有合法的落子位置
//     if (legalMoves.isEmpty()) return; // 如果没有合法的落子位置，直接返回
//     QPoint bestMove = legalMoves[rand() % legalMoves.size()]; // 随机选择一个合法位置
//     map[bestMove.x()][bestMove.y()] = 3 - playerColor; // 放置AI棋子
//     executeFlip(bestMove.x(), bestMove.y(), 3 - playerColor); // 执行翻转
//     update(); // 更新显示
//     isPlayerTurn = true; // 切换到玩家回合
// }

// void SingleWindow::AIMoveMedium() {
//     QVector<QPoint> legalMoves; // 存储所有合法移动
//     findLegalMoves(3 - playerColor, legalMoves); // 为AI玩家收集所有合法移动
//     if (legalMoves.isEmpty()) return; // 如果没有可用的合法移动，直接返回

//     int bestScore = INT_MIN;
//     QPoint bestMove;

//     // 使用minimax算法评估每个移动
//     for (const auto& move : legalMoves) {
//         QVector<QVector<int>> savedState;
//         saveBoardState(savedState); // 保存当前棋盘状态
//         map[move.x()][move.y()] = 3 - playerColor; // 在棋盘上执行此移动
//         executeFlip(move.x(), move.y(), 3 - playerColor);

//         // 使用minimax进行评估：深度、新颜色、alpha、beta、是否为最大化玩家
//         int score = minimax(3, playerColor, INT_MIN, INT_MAX, false); // 中等AI使用深度为3
//         restoreBoardState(savedState); // 恢复棋盘状态

//         // 选择更好的移动
//         if (score > bestScore) {
//             bestScore = score;
//             bestMove = move;
//         }
//     }

//     // 执行最佳移动
//     map[bestMove.x()][bestMove.y()] = 3 - playerColor;
//     executeFlip(bestMove.x(), bestMove.y(), 3 - playerColor);
//     update(); // 重绘棋盘
//     isPlayerTurn = true; // 将回合交还给玩家
// }

// void SingleWindow::AIMoveHard() {
//     QVector<QPoint> legalMoves; // 存储所有合法移动
//     findLegalMoves(3 - playerColor, legalMoves); // 为AI玩家收集所有合法移动
//     if (legalMoves.isEmpty()) return; // 如果没有可用的合法移动，直接返回

//     int bestScore = INT_MIN;
//     QPoint bestMove;

//     // 使用minimax算法评估每个移动
//     for (const auto& move : legalMoves) {
//         QVector<QVector<int>> savedState;
//         saveBoardState(savedState); // 保存当前棋盘状态
//         map[move.x()][move.y()] = 3 - playerColor; // 在棋盘上执行此移动
//         executeFlip(move.x(), move.y(), 3 - playerColor);

//         // 使用minimax进行评估，高级AI使用更大深度
//         int score = minimax(5, playerColor, INT_MIN, INT_MAX, false); // Hard AI使用深度为4
//         restoreBoardState(savedState); // 恢复棋盘状态

//         // 选择更好的移动
//         if (score > bestScore) {
//             bestScore = score;
//             bestMove = move;
//         }
//     }

//     // 执行最佳移动
//     map[bestMove.x()][bestMove.y()] = 3 - playerColor;
//     executeFlip(bestMove.x(), bestMove.y(), 3 - playerColor);
//     update(); // 重绘棋盘
//     isPlayerTurn = true; // 将回合交还给玩家
// }

// bool SingleWindow::isBoardFull() {
//     for (int i = 0; i < sx; ++i) {
//         for (int j = 0; j < sy; ++j) {
//             if (map[i][j] == 0) return false; // 发现有空位，返回false
//         }
//     }
//     return true; // 没有空位，返回true
// }

// void SingleWindow::calculateWinner() {
//     int blackCount = 0;
//     int whiteCount = 0;
//     for (int i = 0; i < sx; ++i) {
//         for (int j = 0; j < sy; ++j) {
//             if (map[i][j] == 1) blackCount++;
//             else if (map[i][j] == 2) whiteCount++;
//         }
//     }

//     QString result;
//     if (blackCount > whiteCount) result = "黑子胜！";
//     else if (whiteCount > blackCount) result = "白子胜！";
//     else result = "平局！";

//     QMessageBox::information(this, "游戏结束", result);
//     resetBoard(); // 重置棋盘
//     update(); // 确保UI更新
// }

// void SingleWindow::checkGameStateAndProceed() {
//     // 检查玩家是否有合法的落子位置
//     bool playerHasMove = false;
//     bool AIHasMove = false;

//     for (int i = 0; i < sx; ++i) {
//         for (int j = 0; j < sy; ++j) {
//             if (isValidMove(i, j, playerColor)) {
//                 playerHasMove = true;
//             }
//             if (isValidMove(i, j, 3 - playerColor)) {
//                 AIHasMove = true;
//             }
//             if (playerHasMove && AIHasMove) break;
//         }
//         if (playerHasMove && AIHasMove) break;
//     }

//     // 如果双方都没有合法移动，或者棋盘已满，结束游戏并计算胜者
//     if ((!playerHasMove && !AIHasMove) || isBoardFull()) {
//         calculateWinner();
//     } else {
//         if (!isPlayerTurn && AIHasMove) {
//             AIMove(ai); // AI执行移动
//             isPlayerTurn = true; // AI完成后，回到玩家回合
//             checkGameStateAndProceed(); // 重新检查游戏状态
//         } else if (!playerHasMove && AIHasMove) {
//             isPlayerTurn = false; // 玩家没有合法移动，继续AI回合
//             checkGameStateAndProceed(); // 重新检查游戏状态
//         }else if (!AIHasMove && isPlayerTurn) {
//             isPlayerTurn = true; // AI没有合法移动，返回玩家回合
//             checkGameStateAndProceed(); // 重新检查游戏状态
//         }
//     }
//     update(); // 更新棋盘显示
// }

// void SingleWindow::saveBoardState(QVector<QVector<int>>& savedState) {
//     savedState.resize(sx); // 确保有足够的行
//     for (int i = 0; i < sx; ++i) {
//         savedState[i].resize(sy);
//         for (int j = 0; j < sy; ++j) {
//             savedState[i][j] = map[i][j]; // 保存当前棋盘状态
//         }
//     }
// }

// void SingleWindow::restoreBoardState(const QVector<QVector<int>>& savedState) {
//     for (int i = 0; i < sx; ++i) {
//         for (int j = 0; j < sy; ++j) {
//             map[i][j] = savedState[i][j]; // 恢复棋盘状态
//         }
//     }
// }

// int SingleWindow::evaluateBoard(int color) {
//     int blackScore = 0;
//     int whiteScore = 0;
//     int blackFrontier = 0;
//     int whiteFrontier = 0;
//     int blackStable = 0;
//     int whiteStable = 0;
//     QVector<QPoint> blackMoves;
//     QVector<QPoint> whiteMoves;

//     // 评估棋盘位置并计算得分
//     for(int i = 0; i < sx; ++i) {
//         for(int j = 0; j < sy; ++j) {
//             if(map[i][j] == 1) {
//                 blackScore += positionWeights[i][j];
//                 if (isFrontier(i, j)) blackFrontier++;
//                 appendValidMoves(i, j, 1, blackMoves);
//                 if (isStable(i, j, 1)) blackStable++;
//             } else if(map[i][j] == 2) {
//                 whiteScore += positionWeights[i][j];
//                 if (isFrontier(i, j)) whiteFrontier++;
//                 appendValidMoves(i, j, 2, whiteMoves);
//                 if (isStable(i, j, 2)) whiteStable++;
//             }
//         }
//     }

//     int mobilityScore = blackMoves.size() - whiteMoves.size();
//     int frontierScore = whiteFrontier - blackFrontier;
//     int stabilityScore = blackStable - whiteStable;
//     int pieceParity = (blackScore + whiteScore) ? 100 * (blackScore - whiteScore) / (blackScore + whiteScore) : 0;

//     // 综合得分
//     int finalScore = 10 * pieceParity + 20 * stabilityScore + 30 * mobilityScore - 5 * frontierScore;

//     // 角落控制
//     int cornerValue = 100;
//     if (map[0][0] == color) finalScore += cornerValue;
//     if (map[0][sx - 1] == color) finalScore += cornerValue;
//     if (map[sy - 1][0] == color) finalScore += cornerValue;
//     if (map[sy - 1][sx - 1] == color) finalScore += cornerValue;

//     return (color == 1) ? finalScore : -finalScore;
// }

// bool SingleWindow::isStable(int x, int y, int color) {
//     // 通过确保所有方向要么是棋盘边缘，要么是相同颜色来检查稳定性
//     static const int directions[8][2] = {
//         {1, 0}, {-1, 0}, {0, 1}, {0, -1},
//         {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
//     };
//     for (const auto& dir : directions) {
//         int nx = x + dir[0];
//         int ny = y + dir[1];
//         if (nx >= 0 && nx < sx && ny >= 0 && ny < sy && map[nx][ny] != color) {
//             return false;
//         }
//     }
//     return true;
// }

// bool SingleWindow::isFrontier(int x, int y) {
//     if (map[x][y] == 0) return false;
//     for (int dx = -1; dx <= 1; ++dx) {
//         for (int dy = -1; dy <= 1; ++dy) {
//             int nx = x + dx;
//             int ny = y + dy;
//             if (nx >= 0 && nx < sx && ny >= 0 && ny < sy && map[nx][ny] == 0)
//                 return true;
//         }
//     }
//     return false;
// }

// void SingleWindow::appendValidMoves(int x, int y, int color, QVector<QPoint>& moves) {
//     for (int dx = -1; dx <= 1; ++dx) {
//         for (int dy = -1; dy <= 1; ++dy) {
//             if (isValidMove(x + dx, y + dy, color) && !moves.contains(QPoint(x + dx, y + dy)))
//                 moves.append(QPoint(x + dx, y + dy)); // 添加合法移动
//         }
//     }
// }

// int SingleWindow::minimax(int depth, int color, int alpha, int beta, bool maximizingPlayer) {
//     if (depth == 0 || isBoardFull()) {
//         return evaluateBoard(color); // 评估当前棋盘
//     }

//     // 生成唯一的哈希值用于表示当前棋盘状态
//     quint64 hash = generateBoardHash();
//     if (transpositionTable.contains(hash)) {
//         return transpositionTable[hash]; // 如果缓存中存在，直接返回
//     }

//     QVector<QPoint> legalMoves;
//     for (int i = 0; i < sx; ++i) {
//         for (int j = 0; j < sy; ++j) {
//             if (isValidMove(i, j, color)) {
//                 legalMoves.append(QPoint(i, j)); // 添加合法移动位置
//             }
//         }
//     }

//     if (legalMoves.isEmpty()) {
//         return evaluateBoard(color); // 如果没有合法移动，则评估当前棋盘
//     }

//     int value;
//     QVector<QVector<int>> savedState;
//     if (maximizingPlayer) {
//         value = INT_MIN;
//         // 对合法移动排序，优先评估更有利的位置
//         std::sort(legalMoves.begin(), legalMoves.end(), [this, color](QPoint a, QPoint b) {
//             return positionWeights[a.x()][a.y()] > positionWeights[b.x()][b.y()];
//         });
//         for (auto &move : legalMoves) {
//             saveBoardState(savedState);
//             executeFlip(move.x(), move.y(), color);
//             map[move.x()][move.y()] = color;

//             value = std::max(value, minimax(depth - 1, 3 - color, alpha, beta, false));
//             restoreBoardState(savedState);
//             alpha = std::max(alpha, value);
//             if (alpha >= beta) break; // Alpha-beta剪枝
//         }
//     } else {
//         value = INT_MAX;
//         std::sort(legalMoves.begin(), legalMoves.end(), [this, color](QPoint a, QPoint b) {
//             return positionWeights[a.x()][a.y()] < positionWeights[b.x()][b.y()];
//         });
//         for (auto &move : legalMoves) {
//             saveBoardState(savedState);
//             executeFlip(move.x(), move.y(), 3 - color);
//             map[move.x()][move.y()] = 3 - color;

//             value = std::min(value, minimax(depth - 1, color, alpha, beta, true));
//             restoreBoardState(savedState);
//             beta = std::min(beta, value);
//             if (beta <= alpha) break; // Alpha-beta剪枝
//         }
//     }
//     transpositionTable[hash] = value; // 缓存评估结果
//     return value;
// }

// quint64 SingleWindow::generateBoardHash() {
//     quint64 hash = 0;
//     quint64 prime = 31;
//     for (int i = 0; i < sx; ++i) {
//         for (int j = 0; j < sy; ++j) {
//             hash = hash * prime + map[i][j];
//         }
//     }
//     return hash;
// }

// SingleWindow::~SingleWindow()
// {
//     delete ui;
// }
