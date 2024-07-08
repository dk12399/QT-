#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mutiwindow.h"
#include "singlewindow.h"
#include "helpwindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // 设置 UI
    setWindowTitle("黑白棋"); // 设置窗口标题
    setFixedSize(800, 800); // 设置窗口大小为 800x800
    // 创建一个中心窗口
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget); // 设置中心窗口

    // 使用 QVBoxLayout（垂直）
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setAlignment(Qt::AlignCenter); // 居中对齐
    layout->setSpacing(20); // 设置按钮之间的间距

    // 添加背景图片
    backgroundLabel = new QLabel(this);
    QPixmap backgroundPixmap(":/images/D:/Google/Download/othello_cover_final2.png"); // 确保你的资源文件中有该图片
    backgroundLabel->setPixmap(backgroundPixmap); // 设置背景图片
    backgroundLabel->setScaledContents(true); // 背景图像适应窗口大小
    backgroundLabel->setGeometry(0, 0, this->width(), this->height()); // 设置全屏背景

    // 添加按钮并设置样式
    QPushButton *buttonSingle = new QPushButton("单人模式", this);
    buttonSingle->setFixedSize(200, 50); // 设置按钮固定大小
    buttonSingle->setToolTip("开始单人游戏模式"); // 设置工具提示
    buttonSingle->setStyleSheet(R"(
        QPushButton {
            font-size: 18px;
            color: white;
            background-color: #3498db;
            border: none;
            border-radius: 10px;
        }
        QPushButton:hover {
            background-color: #2980b9;
        }
        QPushButton:pressed {
            background-color: #1c6398;
        }
    )"); // 设置按钮样式
    layout->addWidget(buttonSingle); // 将按钮添加到布局中
    connect(buttonSingle, &QPushButton::clicked, this, &MainWindow::OpenSingleWindow); // 连接按钮的点击信号到槽函数

    QPushButton *buttonMuti = new QPushButton("双人对战", this);
    buttonMuti->setFixedSize(200, 50); // 设置按钮固定大小
    buttonMuti->setToolTip("开始双人对战模式"); // 设置工具提示
    buttonMuti->setStyleSheet(R"(
        QPushButton {
            font-size: 18px;
            color: white;
            background-color: #e74c3c;
            border: none;
            border-radius: 10px;
        }
        QPushButton:hover {
            background-color: #c0392b;
        }
        QPushButton:pressed {
            background-color: #96281b;
        }
    )"); // 设置按钮样式
    layout->addWidget(buttonMuti); // 将按钮添加到布局中
    connect(buttonMuti, &QPushButton::clicked, this, &MainWindow::OpenMutiWindow); // 连接按钮的点击信号到槽函数

    QPushButton *buttonHelp = new QPushButton("帮助", this); // 添加帮助按钮
    buttonHelp->setFixedSize(200, 50); // 设置按钮固定大小
    buttonHelp->setToolTip("查看游戏规则和帮助"); // 设置工具提示
    buttonHelp->setStyleSheet(R"(
        QPushButton {
            font-size: 18px;
            color: white;
            background-color: #2ecc71;
            border: none;
            border-radius: 10px;
        }
        QPushButton:hover {
            background-color: #27ae60;
        }
        QPushButton:pressed {
            background-color: #1e8449;
        }
    )"); // 设置按钮样式
    layout->addWidget(buttonHelp); // 将按钮添加到布局中
    connect(buttonHelp, &QPushButton::clicked, this, &MainWindow::OpenHelpWindow); // 连接按钮的点击信号到槽函数

    // 将背景标签置于最底层
    backgroundLabel->lower();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event); // 调用基类的 resizeEvent
    backgroundLabel->setGeometry(0, 0, this->width(), this->height()); // 调整背景标签大小
}

void MainWindow::OpenMutiWindow() {
    MutiWindow *mutiWindow = new MutiWindow(); // 创建 MutiWindow 实例
    mutiWindow->show(); // 显示 MutiWindow 界面
    close(); // 关闭当前窗口
}

void MainWindow::OpenSingleWindow()
{
    SingleWindow *singleWindow = new SingleWindow(); // 创建 SingleWindow 实例
    singleWindow->show(); // 显示 SingleWindow 界面
    close(); // 关闭当前窗口
}

void MainWindow::OpenHelpWindow()
{
    HelpWindow *helpWindow = new HelpWindow(); // 创建 HelpWindow 实例
    connect(helpWindow, &HelpWindow::backToMain, this, [this]() {
        show(); // 显示主窗口
    });
    helpWindow->show(); // 显示 HelpWindow 界面
    hide(); // 隐藏当前窗口
}

MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 资源
}
