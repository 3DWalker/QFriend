#include "briefwidget.h"
#include "ui_briefwidget.h"

BriefWidget::BriefWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BriefWidget)
{
    ui->setupUi(this);
}

BriefWidget::~BriefWidget()
{
    delete ui;
}

void BriefWidget::handleFriendBrief(QString FriendName)
{
    ui->lb_friendName->setText(FriendName);
}

void BriefWidget::on_pushButton_clicked()
{
    emit mySignal(ui->lb_friendName->text());
}

