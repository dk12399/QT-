#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    // 析构函数
    ~MainWindow();

protected:
    // 重写 resizeEvent 事件处理函数，用于处理窗口大小改变事件
    void resizeEvent(QResizeEvent *event) override;

private slots:
    // 槽函数：打开双人对战
    void OpenMutiWindow();
    // 槽函数：打开单人对战
    void OpenSingleWindow();
    // 槽函数：打开帮助
    void OpenHelpWindow();

private:
    Ui::MainWindow *ui;
    QLabel *backgroundLabel;
};

#endif // MAINWINDOW_H
