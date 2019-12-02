#include "historywidget.h"
#include "ui_historywidget.h"

HistoryWidget::HistoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryWidget)
{
    ui->setupUi(this);
}

HistoryWidget::~HistoryWidget()
{
    delete ui;
}
