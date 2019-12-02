#include "startwidget.h"
#include "ui_startwidget.h"

StartWidget::StartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    connect(ui->start,SIGNAL(clicked()),this,SIGNAL(start()));
    connect(ui->history,SIGNAL(clicked()),this,SIGNAL(history()));
}

StartWidget::~StartWidget()
{
    delete ui;
}
