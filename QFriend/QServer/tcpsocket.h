#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include "database.h"
#include "protocol.h"

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
private:
    DatabaseManager *m_database;

    QString m_userName;

public:
    TcpSocket();

    DatabaseManager *getDatabase();

    QString getUserName();

public slots:
    void recvMsg();
};

#endif // TCPSOCKET_H
