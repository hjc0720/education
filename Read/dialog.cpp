#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->select,&SelectDlg::add,[this](){
        ui->stackedWidget->setCurrentIndex(1);
    });
}

Dialog::~Dialog()
{
    delete ui;
}
