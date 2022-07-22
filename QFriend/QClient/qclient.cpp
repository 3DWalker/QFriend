#include "qclient.h"
#include "ui_qclient.h"
#include "basewidget.h"

QClient::QClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QClient)
{
    ui->setupUi(this);
    loadConfig();

    m_tcpSocket.connectToHost(QHostAddress(m_IP), m_Port);

    connect(&m_tcpSocket, SIGNAL(connected()), this, SLOT(connectSuccess()));
    connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(recvMsg()));

}

QClient::~QClient()
{
    delete ui;
}

// 加载配置文件
void QClient::loadConfig()
{
    QFile file(":/client.config");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        QString strData = data.toStdString().c_str();

        strData.replace("\n", " ");
        QStringList strList = strData.split(" ");
        m_IP = strList[0];
        m_Port = strList[1].toUShort();
    }
}

QClient &QClient::getInstance()
{
    static QClient instance;
    return instance;
}



//获取socket实例
QTcpSocket &QClient::getTcpSocket()
{
    return m_tcpSocket;
}

QString QClient::getLocalName()
{
    return m_userName;
}

//连接服务器成功消息提示框
void QClient::connectSuccess()
{
//    QMessageBox::information(this, "连接服务器", "连接成功!");
}

//信号槽--注册
void QClient::on_pb_regist_clicked()
{
    QString strName = ui->le_name->text();
    QString strPswd = ui->le_pswd->text();
    if (!strName.isEmpty() && !strPswd.isEmpty())
    {
        PDU *pdu = mkPDU(0);
        pdu->usPDUType = ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->usData, strName.toStdString().c_str(), 32);
        strncpy(pdu->usData + 32, strPswd.toStdString().c_str(), 32);

        m_tcpSocket.write((char *)pdu, pdu->usPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this, "注册失败", "用户名或密码为空");
    }
}

//信号槽--登录
void QClient::on_pb_login_clicked()
{
    QString strName = ui->le_name->text();
    QString strPswd = ui->le_pswd->text();
    if (!strName.isEmpty() && !strPswd.isEmpty())
    {
        //存储用户名
        m_userName = strName;

        PDU *pdu = mkPDU(0);
        pdu->usPDUType = ENUM_MSG_TYPE_LOGIN_REQUEST;
        strncpy(pdu->usData, strName.toStdString().c_str(), 32);
        strncpy(pdu->usData + 32, strPswd.toStdString().c_str(), 32);

        m_tcpSocket.write((char *)pdu, pdu->usPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this, "登录失败", "用户名或密码为空");
    }
}

//信号槽--接收信息
void QClient::recvMsg()
{
    qDebug() << m_tcpSocket.bytesAvailable();
    uint pduLen = 0;
    m_tcpSocket.read((char *)&pduLen, sizeof(uint));

    PDU *pdu = mkPDU(pduLen - sizeof(PDU));
    m_tcpSocket.read((char *)pdu + sizeof(uint), pduLen - sizeof(uint));

    uint msgLen = pduLen - sizeof(PDU);
    if (msgLen != pdu->usMsgLen)
    {
        qDebug() << "msgLen error!";
    }
    switch (pdu->usPDUType)
    {
    //注册响应
    case ENUM_MSG_TYPE_REGIST_RESPOND:
    {
        if (0 == strcmp(pdu->usData, USER_REGIST_SUCCEED))
        {
            QMessageBox::information(this, "注册", "注册成功");
        }
        else if (0 == strcmp(pdu->usData, USER_REGIST_FAILED))
        {
            QMessageBox::information(this, "注册", "注册失败");
        }
        else
        {
            QMessageBox::information(this, "注册", "注册失败");
        }
        break;
    }
    //登录响应
    case ENUM_MSG_TYPE_LOGIN_RESPOND:
    {
        if (0 == strcmp(pdu->usData, USER_LOGIN_SUCCEED))
        {
            BaseWidget::getInstance().show();
            BaseWidget::getInstance().setWindowTitle(m_userName);
            hide();

            PDU *reqpdu = mkPDU(0);
            reqpdu->usPDUType = ENUM_MSG_TYPE_SHOW_ALL_FRIEND_REQUEST;
            strncpy(reqpdu->usData, m_userName.toStdString().c_str(), 32);
            m_tcpSocket.write((char *)reqpdu, reqpdu->usPDULen);
            free(reqpdu);
            reqpdu = NULL;
        }
        else
        {
            QMessageBox::information(this, "登录", pdu->usData);
        }
        break;
    }
    //好友申请失败回应
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:
    {
        QMessageBox::information(this, "添加好友", pdu->usData);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
    {
        BaseWidget::getInstance().getFriendWidget()->handleAddFriend(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE:
    case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE:
    {
        BaseWidget::getInstance().getFriendWidget()->handleAddFriendRespond(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SHOW_ALL_FRIEND_RESPOND:
    {
        BaseWidget::getInstance().getFriendWidget()->handleShowAllFriendRespond(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SEND_MSG_RESPOND:
    {
        BaseWidget::getInstance().getMsgWidget()->handleAddMessageToList(pdu);
        break;
    }
    default:
        break;
    }
}

