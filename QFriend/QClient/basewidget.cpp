#include "basewidget.h"
#include "ui_basewidget.h"
#include "qclient.h"

BaseWidget::BaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseWidget)
{
    ui->setupUi(this);

    friendwd = new FriendWidget();
    msgwd = new MsgWidget();
    ui->stackedWidget->addWidget(friendwd);
    ui->stackedWidget->addWidget(msgwd);
    ui->stackedWidget->setCurrentWidget(friendwd);

    connect(friendwd, SIGNAL(changeStackedWidgetSignal(QString)), this, SLOT(changeStackedWidget(QString)));
}

BaseWidget::~BaseWidget()
{
    delete ui;
}

BaseWidget &BaseWidget::getInstance()
{
    static BaseWidget instance;
    return instance;
}

FriendWidget *BaseWidget::getFriendWidget()
{
    return friendwd;
}

MsgWidget *BaseWidget::getMsgWidget()
{
    return msgwd;
}

void BaseWidget::changeStackedWidget(QString itemName)
{
    msgwd->handleChatObject(itemName);
    ui->stackedWidget->setCurrentWidget(msgwd);
}

void BaseWidget::on_pb_msg_clicked()
{
    ui->stackedWidget->setCurrentWidget(msgwd);
}


void BaseWidget::on_pb_friend_clicked()
{
    ui->stackedWidget->setCurrentWidget(friendwd);
}

