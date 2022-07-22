#ifndef QCLIENT_H
#define QCLIENT_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "protocol.h"
//#include "basewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QClient; }
QT_END_NAMESPACE

class QClient : public QWidget
{
    Q_OBJECT

public:
    ~QClient();

    void loadConfig();

    static QClient &getInstance();

    QTcpSocket &getTcpSocket();

    QString getLocalName();

public slots:
    void connectSuccess();

    void recvMsg();

private slots:

    void on_pb_regist_clicked();

    void on_pb_login_clicked();

private:
    QClient(QWidget *parent = nullptr);
    Ui::QClient *ui;
    QString m_IP;
    qint16 m_Port;

    QTcpSocket m_tcpSocket;

    QString m_userName;
};
#endif // QCLIENT_H
