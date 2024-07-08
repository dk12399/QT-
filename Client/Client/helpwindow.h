#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class HelpWindow : public QWidget
{
    Q_OBJECT

public:
    HelpWindow(QWidget *parent = nullptr);
    ~HelpWindow();

private slots:
    void goBackToMain(); // 返回首页的槽函数
    void navigateToSection(QListWidgetItem *item); // 导航到指定章节
    void onContentReceived(QNetworkReply* reply); // 处理网络回复的槽函数

signals:
    void backToMain(); // 返回首页的信号

private:
    QTextEdit *textEdit; // 将 textEdit 声明为成员变量
    QListWidget *navList; // 导航栏
    QTimer *updateTimer; // 定时器用于动态更新内容
    QNetworkAccessManager *networkManager; // 网络管理器用于获取内容
};

#endif // HELPWINDOW_H
