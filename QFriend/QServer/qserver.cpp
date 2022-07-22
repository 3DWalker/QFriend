#include "qserver.h"
#include "ui_qserver.h"
#include "tcpserver.h"

QServer::QServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QServer)
{
    ui->setupUi(this);
    loadConfig();

    TcpServer::getInstance().listen(QHostAddress(m_IP), m_Port);
}

QServer::~QServer()
{
    delete ui;
}

//加载配置文件
void QServer::loadConfig()
{
    QFile file(":/server.config");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        QString strData = data.toStdString().c_str();

        strData.replace("\n", " ");
        QStringList strList = strData.split(" ");
        m_IP = strList[0];
        m_Port = strList[1].toUShort();
    }
    else
    {
        QMessageBox::warning(this, "加载配置文件", "加载失败");
    }
}
