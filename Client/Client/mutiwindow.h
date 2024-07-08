#ifndef MUTIWINDOW_H
#define MUTIWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTcpSocket>
#include <QLineEdit>
#include <QMessageBox>
#include <QStack>
#include <QVector>
#include <QPoint>
#include <QTextEdit>

const int borad_size = 8; // 棋盘大小

namespace Ui {
class MutiWindow;
}

class MutiWindow : public QWidget {
    Q_OBJECT

public:
    explicit MutiWindow(QWidget *parent = nullptr);
    ~MutiWindow();
    int map[borad_size][borad_size]; // 棋盘数组
    //事件函数
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    //绘制函数（通过paintEvent调用
    void paintBoard(QPainter *pt);
    void paintSquare(QPainter *pt);
    void paintChess(QPainter *pt);
    //逻辑函数
    void eat(int x, int y, int color, int dx, int dy, QVector<QPoint> &flipped);//吃子
    bool isBoardFull();//棋盘是否满
    bool isGameOver();//游戏是否结束（判断
    void calculateWinner();//游戏结束（输入结果
    void resetBoard();//初始化棋盘
    bool isValidMove(int x, int y, int color);//落子是否合法
    bool hasValidMove(int color);//是否存在合法移动
    void executeFlip(int x, int y, int color, QVector<QPoint> &flipped);//调用eat函数，反转棋子
    void setPlayerColor(const QString &color);//设置玩家颜色
    void setupUI();//设定UI

    void attemptConnection(const QString &ip, int port);//尝试连接
    void readServer();//读取服务端发送来的数据
    void processMove(int x, int y, int color);//读取服务端传来的数据进行翻转
    void sendMove(int x, int y);//将移动发送给服务端
    void handleUndoRequest();//处理悔棋请求
    void sendUndoResponse(bool accepted);//发送悔棋请求
    void undoAccepted();//接受悔棋
    void handleDrawRequest();//处理求和请求
    void sendDrawResponse(bool accepted);//发送求和请求
    void handleDrawAccepted();  //处理求和接受
    void handleSurrender();//处理认输请求
    void sendSwapColorResponse(bool accepted);  // 如果颜色交换接受，重置棋盘重置颜色
    void handleSwapColorRequest();  // 处理交换颜色请求





private slots:
    void onConnected();//连接成功提示
    void onErrorOccurred(QAbstractSocket::SocketError socketError);//失败提示
    void handleReturnToMain();//处理返回主页面
    void sendMessage();  // 发送聊天相关消息
private:
    Ui::MutiWindow *ui;
    QLabel *colorLabel; // 显示当前颜色的标签
    QLabel *blackCountLabel; // 显示黑棋数量的标签
    QLabel *whiteCountLabel; // 显示白棋数量的标签
    QLabel *turnLabel; // 显示当前回合的标签
    QTextEdit *chatBox; // 聊天记录框
    QLineEdit *chatInput; // 聊天输入框
    int row = -1;
    int col = -1;
    int currentX = -1;
    int currentY = -1;
    int cellSize = 80;
    int boardWidth = borad_size * cellSize;
    int boardHeight = borad_size * cellSize;
    int pieceSize = 55;
    int xOffset;
    int yOffset;
    int playerColor = 0; // 默认未定义颜色
    bool isPlayerTurn = false;
    int lastMoveRow = -1;
    int lastMoveCol = -1;
    QTcpSocket *socket;
    QLabel *connectionStatus;

    QStack<QPair<QPoint, int>> moveHistory; // 记录棋步历史，用于悔棋操作
    QStack<QVector<QPoint>> flipHistory; // 记录每步翻转的棋子历史
    bool hasBlackPlayer = false;  // 是否有黑方玩家
    bool hasWhitePlayer = false;  // 是否有白方玩家

};

#endif // MUTIWINDOW_H
