#include "mainui.h"
#include "network.h"
#include "board.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>

MainUI::MainUI() : QDialog(),
    layout(new QVBoxLayout), boardLayout(new QGridLayout),
    chu(new QLabel("楚河")), han(new QLabel("汉界"))
{
    auto count = 0;
    for (auto& i : h) {
        i = new QLabel("---");
        boardLayout->addWidget(i, count / 8 * 2, count % 8 * 2 + 1, Qt::AlignCenter);
        count++;
    }
    count = 0;
    for (auto& i : v1) {
        i = new QLabel("|");
        boardLayout->addWidget(i, count / 9 * 2 + 1, count % 9 * 2, Qt::AlignCenter);
        count++;
    }
    boardLayout->addWidget(chu, 9, 0, 1, 8, Qt::AlignCenter);
    boardLayout->addWidget(han, 9, 10, 1, 8, Qt::AlignCenter);
    for (auto& i : v2) {
        i = new QLabel("|");
        boardLayout->addWidget(i, count / 9 * 2 + 3, count % 9 * 2, Qt::AlignCenter);
        count++;
    }
    count = 0;
    for (auto& i : cells){
        i = new Cell(count% 9 + 1, 10 - count / 9);
        boardLayout->addWidget(i, count / 9 * 2, count % 9 * 2);
        count++;
    }
    l[0] = new QLabel("\\");
    boardLayout->addWidget(l[0], 1, 7, Qt::AlignCenter);
    l[1] = new QLabel("/");
    boardLayout->addWidget(l[1], 1, 9, Qt::AlignCenter);
    l[2] = new QLabel("/");
    boardLayout->addWidget(l[2], 3, 7, Qt::AlignCenter);
    l[3] = new QLabel("\\");
    boardLayout->addWidget(l[3], 3, 9, Qt::AlignCenter);
    l[0] = new QLabel("\\");
    boardLayout->addWidget(l[0], 15, 7, Qt::AlignCenter);
    l[1] = new QLabel("/");
    boardLayout->addWidget(l[1], 15, 9, Qt::AlignCenter);
    l[6] = new QLabel("/");
    boardLayout->addWidget(l[6], 17, 7, Qt::AlignCenter);
    l[7] = new QLabel("\\");
    boardLayout->addWidget(l[7], 17, 9, Qt::AlignCenter);

    layout->addLayout(boardLayout);

    //Message Area UI:
    QLabel* message_area = new QLabel;
    QHBoxLayout* send_area = new QHBoxLayout;
    QLineEdit* text = new QLineEdit;
    QPushButton* button = new QPushButton("发送");
    send_area->addWidget(text);
    text->setMinimumWidth(0.5 * width());
    text->setMaximumWidth(0.7 * width());
    send_area->addWidget(button, Qt::AlignRight);
    button->setMaximumWidth(0.2 * width());
    layout->addWidget(message_area);
    layout->addLayout(send_area);

    //认输按钮
    QPushButton* surrender = new QPushButton;
    surrender->setText("认输");
    send_area->addWidget(surrender,Qt::AlignRight);
    surrender->setMaximumWidth(0.2 * width());
    connect(surrender,&QPushButton::clicked,Network::getInstance(),[=](){
        Network::getInstance()->sendMessage("认输");
        QTimer::singleShot(5000,this,&MainUI::onExit);
    });


    setLayout(layout);
    //Add your own code here:
    /////////////////////////

    //背景图片
    this->setStyleSheet("QDialog{border-image:url(:/res/mainui_background.jpg)}");

    //找到对手
    connect(Network::getInstance(),&Network::findOK,this,&MainUI::onFind);
    connect(this,&MainUI::setup,Board::getBoard(),&Board::onSetup);

    //接收聊天
    connect(Network::getInstance(),&Network::message,this,[=](QString message){
        message_area->clear();
        message_area->setText(message);
    });

    //发送聊天
    connect(button,&QPushButton::clicked,Network::getInstance(),[=](){
        Network::getInstance()->Message = text->text();
        Network::getInstance()->sendMessage(Network::getInstance()->Message);
        text->clear();
    });

    for(auto& i : cells)
    {
        connect(i,&Cell::click,Board::getBoard(),&Board::onClick);
    }

    /////////////////////////
}
