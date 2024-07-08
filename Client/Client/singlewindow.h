#ifndef SINGLEWINDOW_H
#define SINGLEWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStack>
#include <QMessageBox>
#include <QVector>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <QRandomGenerator>
#include <QHash>
const int BOARD_SIZE = 8; // Renamed for consistency and clarity

enum AIMode {
    EASY,   // "简易"
    MEDIUM, // "中等"
    HARD    // "困难"
};


namespace Ui {
class SingleWindow;
}

class SingleWindow : public QWidget {
    Q_OBJECT

public:
    explicit SingleWindow(QWidget *parent = nullptr);
    ~SingleWindow();
    int map[BOARD_SIZE][BOARD_SIZE];
    void returnToMain();//返回主页面
    //绘制函数
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    //绘制棋子棋盘等
    void paintBoard(QPainter *pt);
    void paintSquare(QPainter *pt);
    void paintChess(QPainter *pt);
    //逻辑函数
    void eat(int x, int y, int color, int dx, int dy);//吃子
    bool isBoardFull();//棋盘是否为空
    void calculateWinner();//游戏结束（宣布结果
    void resetBoard();//初始化棋盘
    bool isValidMove(int x, int y, int color);//移动是否合法
    void checkGameStateAndProceed();//处理特殊情况并判断游戏是否已经结束
    void executeFlip(int x, int y, int color);//反转棋子
    void undoMove();//悔棋（

    void setupUI();//设置UI
    void changeColor();//切换颜色并更新棋盘
    void updateStatistics();//更新统计信息
    //AI移动函数
    void aiMove(AIMode mode);//AI移动函数
    void aiMoveEasy();
    void aiMoveMedium();
    void aiMoveHard();
    void changeAiMode(int index);//改变AI模式
    int evaluateBoard(int color);//评估函数
    int minimax(int depth, int color, int alpha, int beta, bool maximizingPlayer);//minimax函数
    void saveBoardState(QVector<QVector<int>>& savedState);//保存当前棋盘状态
    void restoreBoardState(const QVector<QVector<int>>& savedState);//恢复棋盘状态
    bool isFrontier(int x, int y);//是否是边角？
    bool isStable(int x, int y, int color);//是否为稳定子
    void appendValidMoves(int x, int y, int color, QVector<QPoint>& moves);//可移动位置
    void findLegalMoves(int color, QVector<QPoint>& legalMoves);//找到合法移动
    quint64 generateBoardHash();//生成哈希表
    void checkAndCleanHashTable();//检查并情况Hash表
    int evaluateMove(const QPoint& move, int color, int depth);//评估移动的得分
    void performMove(int x, int y, int color);//performMove执行移动？
    void selectBestMove(int color, int depth);//找出最好的移动

private:
    Ui::SingleWindow *ui; // UI 界面对象指针
    int row = -1; // 当前移动的行索引，初始值为 -1 表示无效
    int col = -1; // 当前移动的列索引，初始值为 -1 表示无效
    int currentX = -1; // 当前鼠标所在的格子横坐标，初始值为 -1 表示无效
    int currentY = -1; // 当前鼠标所在的格子纵坐标，初始值为 -1 表示无效
    int players = 0; // 玩家棋子数量计数
    int AIs = 0; // AI 棋子数量计数
    int cellSize = 80; // 每个棋盘格子的大小
    int boardWidth = BOARD_SIZE * cellSize; // 棋盘宽度（格子大小乘以格子数量）
    int boardHeight = BOARD_SIZE * cellSize; // 棋盘高度（格子大小乘以格子数量）
    int pieceSize = 55; // 棋子的大小
    int xOffset; // 棋盘相对于窗口的横向偏移
    int yOffset; // 棋盘相对于窗口的纵向偏移
    AIMode ai = EASY; // 当前 AI 的难度模式，默认为简易模式
    int playerColor = 1; // 玩家颜色，1 表示黑子，2 表示白子，默认黑子
    bool isPlayerTurn = true; // 标识是否为玩家回合，默认是玩家回合

    int lastMoveRow = -1; // 最后落子的行索引，初始值为 -1 表示无效
    int lastMoveCol = -1; // 最后落子的列索引，初始值为 -1 表示无效
    QComboBox *aiSwitch; // 用于选择 AI 难度的下拉框
    QPushButton *colorChangeButton; // 用于切换玩家颜色的按钮
    QPushButton *undoButton; // 用于悔棋的按钮
    QLabel *currentDifficultyLabel; // 显示当前 AI 难度的标签
    QStack<QVector<QVector<int>>> historyStack; // 存储棋盘状态的历史堆栈，用于悔棋
    QHash<quint64, int> transpositionTable; // 置换表，用于存储棋盘状态的哈希值和对应的评估分数

    // 棋盘位置的权重数组，用于评估棋盘状态时计算分数
    const int positionWeights[BOARD_SIZE][BOARD_SIZE] = {
        {100, -20, 10, 5, 5, 10, -20, 100},
        {-20, -50, -2, -2, -2, -2, -50, -20},
        {10, -2, -1, -1, -1, -1, -2, 10},
        {5, -2, -1, 0, 0, -1, -2, 5},
        {5, -2, -1, 0, 0, -1, -2, 5},
        {10, -2, -1, -1, -1, -1, -2, 10},
        {-20, -50, -2, -2, -2, -2, -50, -20},
        {100, -20, 10, 5, 5, 10, -20, 100}
    };
};

#endif // SINGLEWINDOW_H
