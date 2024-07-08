#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QValidator>
#include <QRegularExpressionValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startServer(); // 槽函数，用于启动服务器

private:
    Ui::MainWindow *ui;
    QLineEdit *ipAddressInput; // IP地址输入框
    QLineEdit *portInput; // 端口输入框
    QPushButton *startButton; // 开启服务器的按钮
    QLabel *statusLabel; // 状态显示标签
};

#endif // MAINWINDOW_H
