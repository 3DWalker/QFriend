#include "friendwidget.h"
#include "ui_friendwidget.h"

FriendWidget::FriendWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendWidget)
{
    ui->setupUi(this);
    briefwd = new BriefWidget();
    ui->stackedWidget->addWidget(briefwd);

    connect(briefwd, SIGNAL(mySignal(QString)), this, SLOT(showMsgWidget(QString)));
}

FriendWidget::~FriendWidget()
{
    delete ui;
}

//处理好友申请
void FriendWidget::handleAddFriend(PDU *pdu)
{
    char srcName[32] = {0};
    strncpy(srcName, pdu->usData, 32);

    PDU *respdu = mkPDU(0);

    int res = QMessageBox::question(this, "添加好友", QString("%1添加你为好友，是否同意?").arg(srcName));
    if (QMessageBox::Yes == res)
    {
        respdu->usPDUType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE;
        ui->lw_friend->addItem(srcName);
    }
    else
    {
        respdu->usPDUType = ENUM_MSG_TYPE_ADD_FRIEND_REFUSE;
    }
    memcpy(respdu->usData, pdu->usData, sizeof(pdu->usData));
    QClient::getInstance().getTcpSocket().write((char *)respdu, respdu->usPDULen);

    free(respdu);
    pdu = NULL;
}

//处理添加好友回应
void FriendWidget::handleAddFriendRespond(PDU *pdu)
{
    char perName[32] = {0};
    strncpy(perName, pdu->usData + 32, 32);
    if (pdu->usPDUType == ENUM_MSG_TYPE_ADD_FRIEND_AGREE)
    {
        QMessageBox::information(this, "添加好友", QString("添加%1为好友成功").arg(perName));
        ui->lw_friend->addItem(perName);
    }
    else if (ENUM_MSG_TYPE_ADD_FRIEND_REFUSE)
    {
        QMessageBox::information(this, "添加好友", QString("%1拒绝添加好友").arg(perName));
    }
}

void FriendWidget::handleShowAllFriendRespond(PDU *pdu)
{
    if (NULL == pdu)
    {
        return;
    }
    ui->lw_friend->clear();
    QString localName = QClient::getInstance().getLocalName();
    uint userCount = pdu->usMsgLen/32;
    char userName[32] = {0};
    for (uint i = 0; i < userCount; i++)
    {
        memset(userName, 0, sizeof(userName));
        memcpy(userName, (char *)(pdu->usMsg) + i * 32 , 32);
        if (!strcmp(localName.toStdString().c_str(), userName))
        {
            continue;
        }
        ui->lw_friend->addItem(userName);
    }
}

//信号槽--添加好友
void FriendWidget::on_pd_addFriend_clicked()
{
    QString name = QInputDialog::getText(this, "添加好友", "请输入用户名");
    if (!name.isEmpty())
    {
        QString localName = QClient::getInstance().getLocalName();
        PDU *pdu = mkPDU(0);
        pdu->usPDUType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
        strncpy(pdu->usData, localName.toStdString().c_str(), 32);
        strncpy(pdu->usData + 32, name.toStdString().c_str(), 32);

        QClient::getInstance().getTcpSocket().write((char *)pdu, pdu->usPDULen);

        free(pdu);
        pdu = NULL;
    }
}

//信号槽--好友信息简介
void FriendWidget::on_lw_friend_itemClicked(QListWidgetItem *item)
{
    ui->stackedWidget->setCurrentWidget(briefwd);
    briefwd->handleFriendBrief(item->text());
}

void FriendWidget::showMsgWidget(QString itemName)
{
    emit changeStackedWidgetSignal(itemName);
}

