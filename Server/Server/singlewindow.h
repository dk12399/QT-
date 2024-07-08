// #ifndef SINGLEWINDOW_H
// #define SINGLEWINDOW_H

// #include <QWidget>
// #include<QPainter>
// #include<QMouseEvent>
// #include<QLabel>
// #include <QComboBox>
// #include <QPushButton>
// const int sx=8;
// const int sy=8;
// //ai难度
// enum AIMode{
//     EASY,
//     MEDIUM,
//     HARD
// };
// namespace Ui {
// class SingleWindow;
// }

// class SingleWindow : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit SingleWindow(QWidget *parent = nullptr);
//     ~SingleWindow();
//     int map[sx][sy]; // 棋盘存储黑白子

//     //********************事件函数***********************//
//     void paintEvent(QPaintEvent *event); // 调用绘制函数
//     void mousePressEvent(QMouseEvent *event); // 鼠标按下函数（处理落子）
//     void mouseMoveEvent(QMouseEvent *event); // 鼠标移动函数（用来显示你移到什么地方）
//     //*************************************************//

//     //*******************绘制函数************************//
//     void paintboard(QPainter *pt);
//     void paintSquare(QPainter *pt);
//     void paintChess(QPainter *pt);
//     //********************规则函数**********************//
//     void eat(int x,int y,int color,int dx, int dy); // 处理吃子逻辑
//     void gameover(); // 处理结果函数（判断是否结束）
//     bool isBoardFull(); // 棋盘是否已满
//     void calculateWinner(); // 计算胜利者
//     void resetBoard(); // 初始化棋盘
//     bool isValidMove(int x,int y,int color); // 是否可以落子
//     void checkGameStateAndProceed(); // 判断游戏是否结束、落子
//     void executeFlip(int x, int y, int color); // 反转棋子
//     //*************************************************//

//     //*******************UI函数**********************//
//     void setupUI(); // 初始化各种元素
//     void changedAiMode(int index); // 改变AI难度 通过下拉框
//     void changedColor(); // 改变玩家颜色 棋子 和 按钮显示
//     //*************************************************//

//     int colorCount(); // 计算棋子数量,并返回黑白总数

//     //**********************AI相关函数*****************//
//     void AIMove(AIMode mode); // ai移动
//     void AIMoveEasy(); // 简单难度AI
//     void AIMoveHard();
//     void AIMoveMedium();
//     int evaluateBoard(int color); // 估值函数
//     int minimax(int depth, int color, int alpha, int beta, bool maximizingPlayer); // minimax算法
//     void saveBoardState(QVector<QVector<int>>& savedState);
//     void restoreBoardState(const QVector<QVector<int>>& savedState);
//     bool isFrontier(int x, int y);
//     bool isStable(int x, int y, int color);
//     void appendValidMoves(int x, int y, int color, QVector<QPoint>& moves);
//     void findLegalMoves(int color,QVector<QPoint> &legalMoves);//查找合法位置
//     quint64 generateBoardHash();//函数生成唯一的哈希值来表示当前棋盘状态
//     //*************************************************//

// private:
//     Ui::SingleWindow *ui;
//     int row = -1;
//     int col = -1;
//     int currentX = -1; // 更新鼠标当前所在的格子位置
//     int currentY = -1;
//     int players = 0; // 玩家棋子数量
//     int AIs = 0; // ai棋子数量
//     int boardSize = 8; // 棋盘大小
//     int cellSize = 80; // 每个格子的大小
//     int boardWidth = boardSize * cellSize; // 棋盘宽度
//     int boardHeight = boardSize * cellSize; // 棋盘高度
//     int pieceSize = 55; // 棋子的大小
//     int xOffset; // 水平居中的偏移量
//     int yOffset; // 垂直偏移量
//     AIMode ai = EASY; // ai当前难度是？默认为easy
//     int playerColor = 1; // 玩家颜色,黑色or白色， 1 黑 2 白 默认黑色进
//     int stage = 0;
//     bool isPlayerTurn = true; // 是否是玩家回合

//     QComboBox *AISwitch; // 下拉框 改变AI难度
//     QPushButton *ColorChanged; // 改变玩家颜色按钮
//     QPushButton *ClickDone; // 落子  绑定处理落子逻辑的函数

//     QHash<quint64, int> transpositionTable;
//     // 定义加权值矩阵
//     const int positionWeights[sx][sy] = {
//         {100, -20, 10, 5, 5, 10, -20, 100},
//         {-20, -50, -2, -2, -2, -2, -50, -20},
//         {10, -2, -1, -1, -1, -1, -2, 10},
//         {5, -2, -1, 0, 0, -1, -2, 5},
//         {5, -2, -1, 0, 0, -1, -2, 5},
//         {10, -2, -1, -1, -1, -1, -2, 10},
//         {-20, -50, -2, -2, -2, -2, -50, -20},
//         {100, -20, 10, 5, 5, 10, -20, 100}
//     };
// };

// #endif // SINGLEWINDOW_H
