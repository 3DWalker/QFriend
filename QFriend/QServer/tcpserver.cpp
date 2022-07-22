#include "tcpserver.h"

TcpServer::TcpServer()
{

}

//单例模式--获取服务器实例
TcpServer &TcpServer::getInstance()
{
    static TcpServer instance;
    return instance;
}

//套接字描述
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "new client connect";

    TcpSocket *tcpSocket = new TcpSocket;
    tcpSocket->setSocketDescriptor(socketDescriptor);

    m_tcpSocketList.append(tcpSocket);
}

//指定客户端发送信息
void TcpServer::sendto(const char *name, PDU *pdu)
{
    if (NULL == name || NULL == pdu)
    {
        return;
    }
    QString strName = name;
    for (int i = 0; i < m_tcpSocketList.size(); i++)
    {
        if (strName == m_tcpSocketList[i]->getUserName())
        {
            m_tcpSocketList[i]->write((char *)pdu, pdu->usPDULen);
            break;
        }
    }
}

