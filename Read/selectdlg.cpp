#include "selectdlg.h"
#include "ui_selectdlg.h"

SelectDlg::SelectDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectDlg)
{
    ui->setupUi(this);
    connect(ui->add,SIGNAL(clicked()),this,SIGNAL(add()));
    connect(ui->test,SIGNAL(clicked()),this,SIGNAL(begin()));
}

SelectDlg::~SelectDlg()
{
    delete ui;
}
