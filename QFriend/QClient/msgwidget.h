#ifndef MSGWIDGET_H
#define MSGWIDGET_H

#include "QtWidgets/qlistwidget.h"
#include <QWidget>
#include "qclient.h"
#include <QMessageBox>

namespace Ui {
class MsgWidget;
}

class MsgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MsgWidget(QWidget *parent = nullptr);
    ~MsgWidget();

    void handleChatObject(QString srcName);

    void handleAddMessageToList(PDU *pdu);

private slots:
    void on_lw_friend_itemClicked(QListWidgetItem *item);

    void on_pb_send_clicked();

    void on_pb_sendMsg_clicked();

private:
    Ui::MsgWidget *ui;
};

#endif // MSGWIDGET_H
