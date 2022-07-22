#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include "friendwidget.h"
#include "msgwidget.h"

namespace Ui {
class BaseWidget;
}

class BaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseWidget(QWidget *parent = nullptr);
    ~BaseWidget();

    static BaseWidget &getInstance();

    FriendWidget *getFriendWidget();

    MsgWidget *getMsgWidget();

private slots:
    void on_pb_msg_clicked();

    void on_pb_friend_clicked();

    void changeStackedWidget(QString itemName);

private:
    Ui::BaseWidget *ui;

    FriendWidget *friendwd;
    MsgWidget *msgwd;
};

#endif // BASEWIDGET_H
