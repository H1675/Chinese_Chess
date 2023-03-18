#include "loginui.h"
#include "dialogui.h"
#include "network.h"
#include "mainui.h"

#include <QDir>
#include <QMessageBox>
#include <QFrame>

LoginUI::LoginUI()
    : layout(new QFormLayout), buttonLayout(new QHBoxLayout),
      user(new QLabel("Name")), passwd(new QLabel("Password")),
      userTxt(new QLineEdit), passwdTxt(new QLineEdit),
      login(new QPushButton("login")), signup(new QPushButton("signup"))
{
    //设置窗口标题 大小
    this->setWindowTitle("中国象棋");
    this->setFixedSize(580,335);
    this->setStyleSheet("QDialog{image:url(:/res/login_background2.png)}");

    //窗口delete
    setAttribute(Qt::WA_DeleteOnClose);


    //登录label
    QLabel* l1 = new QLabel("Login",this);
    l1->setAlignment(Qt::AlignHCenter);
    l1->setStyleSheet("QLabel{font-size:50px;color:white;border-radius:15px}");

    //登录按钮
    login->setStyleSheet("QPushButton{font-size:15px;color:white;background:transparent;}"
                         "QPushButton::pressed{background-color:#6495ED}");

    signup->setStyleSheet("QPushButton{font-size:15px;color:white;background:transparent;}"
                         "QPushButton::pressed{background-color:#6495ED}");

    //用户名和密码
    userTxt->setMaxLength(20);
    user->setStyleSheet("QLabel{color:white;font-size:15px}");
    passwdTxt->setEchoMode(QLineEdit::Password);
    passwd->setStyleSheet("QLabel{color:white;font-size:15px}");

    //frame
    QFrame* f1 = new QFrame(this);
    f1->resize(200,180);
    f1->move(200,85);
    f1->setStyleSheet("background:transparent;border:3px solid white;border-radius:10px");
    f1->show();

    //设置layout
    layout->addRow(user);
    layout->addRow(userTxt);
    layout->addRow(passwd);
    layout->addRow(passwdTxt);
    buttonLayout->addWidget(login);
    buttonLayout->addWidget(signup);
    layout->addRow(buttonLayout);

    //设置gridlayout
    QGridLayout* g1 = new QGridLayout;
    g1->setColumnStretch(0,1);
    g1->setRowStretch(1,1);
    g1->addWidget(l1,1,1,1,3);
    g1->addLayout(layout,3,1,3,3);
    g1->setRowStretch(6,1);
    g1->setColumnStretch(4,1);

    this->setLayout(g1);



    try {
    //Add your own code here:
    /////////////////////////

        connect(signup,&QPushButton::clicked,this,&LoginUI::onSignup);
        connect(Network::getInstance(),&Network::signupOK,this,&LoginUI::signupSuccess);

        connect(login,&QPushButton::clicked,this,&LoginUI::onLogin);
        connect(Network::getInstance(),&Network::loginOK,this,&LoginUI::loginSuccess);

        connect(Network::getInstance(),&Network::failure,this,&LoginUI::onFailure);

    /////////////////////////
    } catch(Exception e) {
        e.process();
    }
}

void LoginUI::onLogin() {
    Network::getInstance()->login(userTxt->text(), passwdTxt->text());
}

void LoginUI::onSignup() {
    Network::getInstance()->signup(userTxt->text(), passwdTxt->text());
}

void LoginUI::loginSuccess()
{
    Network::getInstance()->username = userTxt->text();
    this->disconnect();
    hide();
    MainUI* ui = new MainUI;
    ui->show();
    auto popup = new DialogUI;
    popup->show();
    close();
}

void LoginUI::signupSuccess() {
    QMessageBox* nov = new QMessageBox;
    nov->setText(tr("注册成功！"));
    nov->show();
}

void LoginUI::onFailure(const QString message) {
    QMessageBox* nov = new QMessageBox;
    nov->setText(message);
    nov->show();
}
