#ifndef SELECTDLG_H
#define SELECTDLG_H

#include <QWidget>

namespace Ui {
class SelectDlg;
}

class SelectDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SelectDlg(QWidget *parent = 0);
    ~SelectDlg();

signals:
    void begin();
    void add();
private:
    Ui::SelectDlg *ui;
};

#endif // SELECTDLG_H
