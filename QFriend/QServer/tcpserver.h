#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include "tcpsocket.h"
#include "protocol.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
private:
    TcpServer();

    QList <TcpSocket *> m_tcpSocketList;

public:
    static TcpServer &getInstance();

    virtual void incomingConnection(qintptr socketDescriptor);

    void sendto(const char *name, PDU *pdu);
};

#endif // TCPSERVER_Hxxx
