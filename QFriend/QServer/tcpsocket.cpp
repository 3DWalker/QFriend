#include "tcpsocket.h"
#include "tcpserver.h"

TcpSocket::TcpSocket()
{
    m_database = DatabaseManager::getInstance();

    connect(this, SIGNAL(readyRead()), this, SLOT(recvMsg()));
}


DatabaseManager *TcpSocket::getDatabase()
{
    return m_database;
}

QString TcpSocket::getUserName()
{
    return m_userName;
}

void TcpSocket::recvMsg()
{
    qDebug() << this->bytesAvailable();

    //获取协议的总长度
    uint pduLen = 0;
    this->read((char *)&pduLen, sizeof(uint));

    //获取完整的协议
    PDU *pdu = mkPDU(pduLen - sizeof(PDU));
    this->read((char *)pdu + sizeof(uint), pduLen - sizeof(uint));

    //检验获取的消息负载长度是否正确
    uint msgLen = pduLen -sizeof(PDU);
    if (msgLen != pdu->usMsgLen)
    {
        qDebug() << "msgLen error";
    }

    switch (pdu->usPDUType)
    {
    case ENUM_MSG_TYPE_REGIST_REQUEST:
    {
        //获取用户名和密码
        char name[32] = {0}, pswd[32] = {0};
        strncpy(name, pdu->usData, 32);
        strncpy(pswd, pdu->usData + 32, 32);

        //创建回应协议
        PDU *respdu = mkPDU(0);
        respdu->usPDUType = ENUM_MSG_TYPE_REGIST_RESPOND;

        //添加到数据库
        bool ok = m_database->handleUserRegist(name, pswd);
        if (ok)
        {
            strcpy(respdu->usData, USER_REGIST_SUCCEED);
        }
        else
        {
            strcpy(respdu->usData, USER_REGIST_FAILED);
        }

        this->write((char *)respdu, respdu->usPDULen);

        free(respdu);
        respdu = NULL;

        break;
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
    {
        //获取用户名和密码
        char name[32] = {0}, pswd[32] = {0};
        strncpy(name, pdu->usData, 32);
        strncpy(pswd, pdu->usData + 32, 32);

        //创建回应协议
        PDU *respdu = mkPDU(0);
        respdu->usPDUType = ENUM_MSG_TYPE_LOGIN_RESPOND;

        int res = m_database->handleUserLogin(name, pswd);
        if (0 == res)
        {
            m_userName = name;
            strcpy(respdu->usData, USER_LOGIN_SUCCEED);
        }
        else if (1 == res)
        {
            strcpy(respdu->usData, USER_LOGIN_FAILED_1);
        }
        else if (2 == res)
        {
            strcpy(respdu->usData, USER_LOGIN_FAILED_2);
        }
        else
        {
            strcpy(respdu->usData, USER_LOGIN_FAILED_3);
        }

        this->write((char *)respdu, respdu->usPDULen);

        free(respdu);
        respdu = NULL;

        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
    {
        char localName[32] = {0}, preName[32] = {0};
        strncpy(localName, pdu->usData, 32);
        strncpy(preName, pdu->usData + 32, 32);

        PDU *respdu = mkPDU(0);
        respdu->usPDUType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;

        int res = m_database->handleAddFriend(localName, preName);
        if (0 == res)
        {
            TcpServer::getInstance().sendto(preName, pdu);
        }
        else if (1 == res)
        {
            strcpy(respdu->usData, ADD_FRIEND_FAILED_1);
            this->write((char *)respdu, respdu->usPDULen);
        }
        else if (2 == res)
        {
            strcpy(respdu->usData, ADD_FRIEND_FAILED_2);
            this->write((char *)respdu, respdu->usPDULen);
        }
        else
        {
            strcpy(respdu->usData, ADD_FRIEND_FAILED_3);
            this->write((char *)respdu, respdu->usPDULen);
        }

        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE:
    {
        char srcName[32] = {0}, perName[32] = {0};
        strncpy(srcName, pdu->usData, 32);
        strncpy(perName, pdu->usData + 32, 32);

        int res = m_database->handleAddfriendAgree(srcName, perName);
        if (0 == res)
        {
            TcpServer::getInstance().sendto(srcName, pdu);
        }
        else
        {
            PDU *respdu = mkPDU(0);
            respdu->usPDUType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu->usData, ADD_FRIEND_FAILED_4);
            this->write((char *)respdu, respdu->usPDULen);

            free(respdu);
            respdu = NULL;
        }

        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE:
    {
        char srcName[32] = {0};
        strncpy(srcName, pdu->usData, 32);
        TcpServer::getInstance().sendto(srcName, pdu);
        break;
    }
    case ENUM_MSG_TYPE_SHOW_ALL_FRIEND_REQUEST:
    {
        char srcName[32] = {0};
        strncpy(srcName, pdu->usData, 32);
        QStringList res = m_database->handleShowAllFriend(srcName);

        uint usMsgLen = res.size() * 32;
        PDU *respdu = mkPDU(usMsgLen);
        respdu->usPDUType = ENUM_MSG_TYPE_SHOW_ALL_FRIEND_RESPOND;

        for (int i = 0; i < res.size(); i++)
        {
            memcpy((char *)(respdu->usMsg) + i * 32
                   , res[i].toStdString().c_str()
                   , res[i].size() < 32 ? res[i].size() : 32);
        }
        this->write((char *)respdu, respdu->usPDULen);

        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_SEND_MSG_REQUEST:
    {
        char localName[32] = {0}, desName[32] = {0};
        strncpy(localName, pdu->usData, 32);
        strncpy(desName, pdu->usData + 32, 32);

        pdu->usPDUType = ENUM_MSG_TYPE_SEND_MSG_RESPOND;

        TcpServer::getInstance().sendto(desName, pdu);
        TcpServer::getInstance().sendto(localName, pdu);

        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu = NULL;
}
