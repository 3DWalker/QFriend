#ifndef QSERVER_H
#define QSERVER_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class QServer; }
QT_END_NAMESPACE

class QServer : public QWidget
{
    Q_OBJECT

public:
    QServer(QWidget *parent = nullptr);
    ~QServer();

    void loadConfig();

private:
    Ui::QServer *ui;
    QString m_IP;
    quint16 m_Port;
};
#endif // QSERVER_H
