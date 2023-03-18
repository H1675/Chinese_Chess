#include "dialogui.h"

#include "board.h"
#include "network.h"

#include <QTimer>

DialogUI::DialogUI()
    : QDialog(), layout(new QVBoxLayout),
      start(new QPushButton("开始游戏")), exit(new QPushButton("退出游戏")),
      pending(new QLabel("请耐心等待，正在匹配玩家……"))
{
    layout->addWidget(start);
    layout->addWidget(exit);
    layout->addWidget(pending);
    setLayout(layout);
    //Add your own code below
    //////////////////////////

    //设置界面
    this->setStyleSheet("QDialog{background-image:url(:/res/dialog_background.jpg)}");

    start->setStyleSheet("QPushButton{font-size:15px;background:transparent}"
                         "QPushButton::pressed{background-color:#6495ED}");

    exit->setStyleSheet("QPushButton{font-size:15px;background:transparent}"
                        "QPushButton::pressed{background-color:#6495ED}");

    connect(Network::getInstance(),&Network::findOK,this,&DialogUI::onFind);
    connect(start,&QPushButton::clicked,this,&DialogUI::onStart);
    connect(exit,&QPushButton::clicked,this,&DialogUI::onExit);

    //输赢
    connect(Board::getBoard(),&Board::win,this,&DialogUI::onWin);
    connect(Board::getBoard(),&Board::netwin,this,&DialogUI::Ddefeat);

    //认输
    connect(Network::getInstance(),&Network::defeat,this,&DialogUI::Ddefeat);

    //////////////////////////
    pending->hide();
}

void DialogUI::onStart() {
    Network::getInstance()->findOpp();
    start->setParent(nullptr);
    layout->removeWidget(start);
    delete start;
    start = nullptr;
    exit->setParent(nullptr);
    layout->removeWidget(exit);
    delete exit;
    exit = nullptr;
    pending->show();
}

void DialogUI::onFind(const QString opp_name) {
    pending->setText(QString("您的对手是%1，%2执红先行。").arg(opp_name,Board::getBoard()->side() ? "您" : opp_name));
    this->show();
}

void DialogUI::onWin(bool side) {
    pending->setText(QString("%1胜！").arg(side ? "红方" : "黑方"));
    show();
    Network::getInstance()->sendMessage(QString("%1胜").arg(side ? "红方" : "黑方"));
    QTimer::singleShot(5000, this, &DialogUI::onExit);
}

void DialogUI::Ddefeat(bool side)
{
    pending->setText(QString("%1胜！").arg(side ? "红方" : "黑方"));
    show();
    QTimer::singleShot(5000, this, &DialogUI::onExit);
}
