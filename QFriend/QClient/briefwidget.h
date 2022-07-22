#ifndef BRIEFWIDGET_H
#define BRIEFWIDGET_H

#include <QWidget>

namespace Ui {
class BriefWidget;
}

class BriefWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BriefWidget(QWidget *parent = nullptr);
    ~BriefWidget();

    void handleFriendBrief(QString FriendName);

private slots:
    void on_pushButton_clicked();

signals:
    QString mySignal(QString item);

private:
    Ui::BriefWidget *ui;
};

#endif // BRIEFWIDGET_H
