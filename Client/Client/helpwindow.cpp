#include "helpwindow.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// HelpWindow 构造函数，初始化成员变量并设置窗口属性
HelpWindow::HelpWindow(QWidget *parent)
    : QWidget(parent), textEdit(new QTextEdit(this)), navList(new QListWidget(this)), updateTimer(new QTimer(this)), networkManager(new QNetworkAccessManager(this))
{
    setWindowTitle("黑白棋规则"); // 设置窗口标题
    setFixedSize(700, 600); // 增加窗口宽度以适应导航栏

    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // 创建主布局
    QHBoxLayout *contentLayout = new QHBoxLayout();//  内容布局
    QVBoxLayout *navLayout = new QVBoxLayout();//导航布局

    // 在导航列表中添加项目
    navList->addItem("基本规则");
    navList->addItem("游戏目标");
    navList->addItem("示例");
    navList->addItem("特殊情况");
    navList->addItem("策略建议");

    // 设置导航列表的固定宽度和样式
    navList->setFixedWidth(150);
    navList->setStyleSheet(R"(
        QListWidget {
            font-size: 14px;
            background-color: #ecf0f1;
            border: 1px solid #bdc3c7;
            border-radius: 5px;
        }
        QListWidget::item {
            padding: 10px;
        }
        QListWidget::item:selected {
            background-color: #3498db;
            color: white;
        }
    )");

    textEdit->setReadOnly(true); // 设置文本编辑器为只读
    textEdit->setStyleSheet(R"(
        QTextEdit {
            background-color: #f5f5f5;
            font-family: 'Arial';
            font-size: 14px;
            color: #333;
            padding: 10px;
            border: 1px solid #ccc;
            border-radius: 5px;
        }
        h2 {
            color: #2c3e50;
            text-align: center;+
        }
        h3 {
            color: #34495e;
            margin-top: 20px;
        }
        p, ol, ul, pre {
            margin: 10px 0;
        }
        ol, ul {
            margin-left: 20px;
        }
        pre {
            background-color: #ecf0f1;
            padding: 10px;
            border: 1px solid #bdc3c7;
            border-radius: 5px;
        }
    )");

    // 初始规则内容
    QString rules = R"(
    <h2 id="top">黑白棋规则</h2>
    <p>黑白棋，也称作 <b>Othello</b>，是一种策略棋类游戏。</p>
    <h3 id="basic">基本规则</h3>
    <ol>
        <li>游戏在 <b>8x8</b> 的棋盘上进行，初始时中央有四枚棋子，黑白各两枚，成对角线放置。</li>
        <li>玩家轮流下子，黑棋先行。</li>
        <li>新子必须与已有棋子相邻，并且能够翻转对方的棋子。</li>
        <li>翻转是指将一行、一列或一对角线上夹在自己新下的棋子与已有的棋子之间的对方的棋子翻转为自己的颜色。</li>
        <li>当一方无合法棋步时，另一方继续下子，直到双方都无合法棋步为止。</li>
        <li>棋盘填满后或双方无合法棋步时，棋盘上棋子多的一方获胜。</li>
    </ol>
    <h3 id="goal">游戏目标</h3>
    <p>通过翻转对方棋子，使自己的棋子数量超过对方。</p>
    <h3 id="example">示例</h3>
    <p>以下是一个翻转的例子：</p>
    <p>黑子在 (3, 5) 处下子，翻转 (4, 4) 处的白子：</p>
    <pre>
        1 2 3 4 5 6 7 8
      1 - - - - - - - -
      2 - - - - - - - -
      3 - - - - B - - -
      4 - - - B W - - -
      5 - - - W B - - -
      6 - - - - - - - -
      7 - - - - - - - -
      8 - - - - - - - -
    </pre>
    <p>下子后结果：</p>
    <pre>
        1 2 3 4 5 6 7 8
      1 - - - - - - - -
      2 - - - - - - - -
      3 - - - - B - - -
      4 - - - B B - - -
      5 - - - W B - - -
      6 - - - - - - - -
      7 - - - - - - - -
      8 - - - - - - - -
    </pre>
    <h3 id="special">特殊情况</h3>
    <p>在某些情况下，可能会出现如下特殊情况：</p>
    <ol>
        <li>如果一个玩家在其回合内没有合法的棋步可下，则该玩家必须跳过这个回合。</li>
        <li>当棋盘已满或者双方都无合法棋步时，游戏结束，棋子数量多的一方获胜。</li>
    </ol>
    <h3 id="strategy">策略建议</h3>
    <p>以下是一些策略建议，帮助玩家更好地进行游戏：</p>
    <ul>
        <li><b>控制角落</b>：角落的位置是最稳定的棋子位置，尽量占据角落。</li>
        <li><b>边缘优势</b>：除了角落，边缘位置的棋子也较为稳定，争取占据边缘位置。</li>
        <li><b>避免陷阱</b>：避免下子位置会给对手提供占据角落或边缘的机会。</li>
        <li><b>灵活调整</b>：根据对手的策略，灵活调整自己的下子计划。</li>
        <li><b>计数法则</b>：随时关注棋盘上的棋子数量，合理评估每一步的得失。</li>
        <li><b>中盘控制</b>：游戏中期要控制棋盘的中部，以便在后期有更多的下子选择。</li>
        <li><b>边缘稳定</b>：尽量保持边缘的棋子不被对手翻转。</li>
    </ul>
    )";

    textEdit->setHtml(rules); // 设置初始规则内容

    QPushButton *backButton = new QPushButton("返回首页", this); // 创建返回首页按钮
    backButton->setStyleSheet(R"(
        QPushButton {
            font-size: 16px;
            padding: 10px 20px;
            color: white;
            background-color: #3498db;
            border: none;
            border-radius: 5px;
        }
        QPushButton:hover {
            background-color: #2980b9;
        }
        QPushButton:pressed {
            background-color: #1c6398;
        }
    )"); // 设置按钮样式

    // 将导航列表和文本编辑器添加到布局中
    navLayout->addWidget(navList);
    contentLayout->addLayout(navLayout);
    contentLayout->addWidget(textEdit);

    // 将内容布局和返回按钮添加到主布局中
    mainLayout->addLayout(contentLayout);
    mainLayout->addWidget(backButton);
    mainLayout->setAlignment(backButton, Qt::AlignCenter); // 按钮居中

    // 连接返回按钮的点击信号到槽函数
    connect(backButton, &QPushButton::clicked, this, &HelpWindow::goBackToMain);
    // 连接导航列表项目点击信号到槽函数
    connect(navList, &QListWidget::itemClicked, this, &HelpWindow::navigateToSection);

    // 连接网络请求完成的信号到槽函数
    connect(networkManager, &QNetworkAccessManager::finished, this, &HelpWindow::onContentReceived);

    setLayout(mainLayout); // 设置主布局
}

// HelpWindow 析构函数
HelpWindow::~HelpWindow()
{
}

// 返回首页槽函数
void HelpWindow::goBackToMain()
{
    emit backToMain(); // 发出返回首页信号
    close(); // 关闭帮助窗口
}

// 导航到对应部分槽函数
void HelpWindow::navigateToSection(QListWidgetItem *item)
{
    QString section = item->text();
    if (section == "基本规则") {
        textEdit->scrollToAnchor("basic"); // 滚动到基本规则部分
    } else if (section == "游戏目标") {
        textEdit->scrollToAnchor("goal"); // 滚动到游戏目标部分
    } else if (section == "示例") {
        textEdit->scrollToAnchor("example"); // 滚动到示例部分
    } else if (section == "特殊情况") {
        textEdit->scrollToAnchor("special"); // 滚动到特殊情况部分
    } else if (section == "策略建议") {
        textEdit->scrollToAnchor("strategy"); // 滚动到策略建议部分
    }
}

// 处理网络请求响应槽函数
void HelpWindow::onContentReceived(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll(); // 读取响应数据
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData); // 解析 JSON 数据

        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            QString newContent;

            if (jsonObj.contains("rules")) {
                QJsonObject rulesObj = jsonObj["rules"].toObject();
                newContent += "<h3 id='basic'>基本规则</h3><ol>";
                for (const auto& rule : rulesObj["basic"].toArray()) {
                    newContent += "<li>" + rule.toString() + "</li>";
                }
                newContent += "</ol>";

                newContent += "<h3 id='goal'>游戏目标</h3><p>" + rulesObj["goal"].toString() + "</p>";

                newContent += "<h3 id='example'>示例</h3><p>" + rulesObj["example"].toString() + "</p>";

                newContent += "<h3 id='special'>特殊情况</h3><ol>";
                for (const auto& special : rulesObj["special"].toArray()) {
                    newContent += "<li>" + special.toString() + "</li>";
                }
                newContent += "</ol>";

                newContent += "<h3 id='strategy'>策略建议</h3><ul>";
                for (const auto& strategy : rulesObj["strategy"].toArray()) {
                    newContent += "<li>" + strategy.toString() + "</li>";
                }
                newContent += "</ul>";
            }

            textEdit->setHtml(newContent); // 更新文本编辑器内容
        }
    }
    reply->deleteLater(); // 清理网络回复
}
