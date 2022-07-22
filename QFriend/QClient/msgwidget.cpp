#include "msgwidget.h"
#include "ui_msgwidget.h"
#include "briefwidget.h"

MsgWidget::MsgWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MsgWidget)
{
    ui->setupUi(this);
}

MsgWidget::~MsgWidget()
{
    delete ui;
}

void MsgWidget::handleChatObject(QString srcName)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(srcName);
    QList<QListWidgetItem *> res = ui->lw_friend->findItems(srcName, Qt::MatchExactly);
    if (res.isEmpty())
    {
        ui->lw_friend->addItem(item);
    }
    ui->lw_friend->setCurrentItem(item);
    ui->lb_name->setText(srcName);
}

void MsgWidget::handleAddMessageToList(PDU *pdu)
{
    char srcName[32] = {0}, desName[32] = {0};
    strncpy(srcName, pdu->usData, 32);
    strncpy(desName, pdu->usData + 32, 32);

    QString localName = QClient::getInstance().getLocalName();
    QList <QListWidgetItem *> list1 = ui->lw_friend->findItems(srcName, Qt::MatchExactly);
    QList <QListWidgetItem *> list2 = ui->lw_friend->findItems(desName, Qt::MatchExactly);
    if (localName != srcName && list1.size() == 0)
    {
        ui->lw_friend->addItem(srcName);
    }
    if (localName != desName && list2.size() == 0)
    {
        ui->lw_friend->addItem(desName);
    }

    QString strMsg = (char *)pdu->usMsg;
    ui->te_msg->append(strMsg);
}

void MsgWidget::on_lw_friend_itemClicked(QListWidgetItem *item)
{
    ui->lb_name->setText(item->text().toStdString().c_str());
}

//槽函数--发送信息
void MsgWidget::on_pb_send_clicked()
{
    QString localName = QClient::getInstance().getLocalName();
    QString desName = ui->lb_name->text();
    QString strMsg = ui->le_msg->text();
    if (0 == strMsg.size())
    {
        QMessageBox::information(this, "发送信息", "发送的信息不能为空");
    }

    PDU *pdu = mkPDU(strMsg.size());
    pdu->usPDUType = ENUM_MSG_TYPE_SEND_MSG_REQUEST;
    strncpy(pdu->usData, localName.toStdString().c_str(), 32);
    strncpy(pdu->usData + 32, desName.toStdString().c_str(), 32);
    strncpy((char *)pdu->usMsg, strMsg.toStdString().c_str(), strMsg.size());

    QClient::getInstance().getTcpSocket().write((char *)pdu, pdu->usPDULen);
    free(pdu);
    pdu = NULL;
}

//发送文件
void MsgWidget::on_pb_sendMsg_clicked()
{

}

