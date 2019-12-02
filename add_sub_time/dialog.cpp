#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->start,&StartWidget::start,std::bind(std::mem_fn(&QStackedWidget::setCurrentIndex),ui->stack,1));
    connect(ui->start,SIGNAL(start()),ui->content,SLOT(start()));
    connect(ui->start,&StartWidget::history,std::bind(std::mem_fn(&QStackedWidget::setCurrentIndex),ui->stack,2));
    connect(ui->content,SIGNAL(destroyed()),this,SLOT(close()));
}

Dialog::~Dialog()
{
    delete ui;
}
