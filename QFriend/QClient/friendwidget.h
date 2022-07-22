#ifndef FRIENDWIDGET_H
#define FRIENDWIDGET_H

#include <QWidget>
#include <QInputDialog>
#include "QtWidgets/qlistwidget.h"
#include "qclient.h"
#include "briefwidget.h"

namespace Ui {
class FriendWidget;
}

class FriendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FriendWidget(QWidget *parent = nullptr);
    ~FriendWidget();

    void handleAddFriend(PDU *pdu);

    void handleAddFriendRespond(PDU *pdu);

    void handleShowAllFriendRespond(PDU *pdu);

private slots:
    void on_pd_addFriend_clicked();

    void on_lw_friend_itemClicked(QListWidgetItem *item);

    void showMsgWidget(QString itemName);

signals:
    void changeStackedWidgetSignal(QString);

private:
    Ui::FriendWidget *ui;
    BriefWidget *briefwd;

};

#endif // FRIENDWIDGET_H
