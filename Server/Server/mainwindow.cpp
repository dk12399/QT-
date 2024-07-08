#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Servering.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("黑白棋游戏--服务器端");
    setFixedSize(500, 300);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QGridLayout *layout = new QGridLayout(centralWidget);

    QLabel *ipLabel = new QLabel("IP 地址:", this);
    ipAddressInput = new QLineEdit(this);
    QLabel *portLabel = new QLabel("端口:", this);
    portInput = new QLineEdit(this);
    startButton = new QPushButton("开启端口", this);
    statusLabel = new QLabel("准备就绪", this);

    QRegularExpression ipRegex("^((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$");
    QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(ipRegex, this);
    ipAddressInput->setValidator(ipValidator);
    QIntValidator *portValidator = new QIntValidator(1, 65535, this);
    portInput->setValidator(portValidator);

    layout->addWidget(ipLabel, 0, 0);
    layout->addWidget(ipAddressInput, 0, 1);
    layout->addWidget(portLabel, 1, 0);
    layout->addWidget(portInput, 1, 1);
    layout->addWidget(startButton, 2, 0, 1, 2);
    layout->addWidget(statusLabel, 3, 0, 1, 2);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startServer);
}

void MainWindow::startServer()
{
    QString ipAddress = ipAddressInput->text();
    int port = portInput->text().toInt();
    if (ipAddress.isEmpty() || port == 0) {
        statusLabel->setText("请输入有效的IP地址和端口");
        return;
    }
    Servering *Server= new Servering();
    if (Server->startServer(ipAddress, port)) {
        statusLabel->setText("服务器启动成功");
        Server->show();
        this->hide(); // 隐藏当前窗口
    } else {
        statusLabel->setText("服务器启动失败：" + Server->getError());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
