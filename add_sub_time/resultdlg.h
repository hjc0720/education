#ifndef RESULTDLG_H
#define RESULTDLG_H

#include <QDialog>
#include <QPolygonF>

namespace Ui {
class ResultDlg;
}

class ResultDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDlg(QWidget *parent = 0);
    ~ResultDlg();

    void setRightCount(int nCount);
    void setUseTime(int nSeconds);
    void setStars(int nStars);
protected:
    int     m_nStart;
    QPolygonF starPolygon;

private:
    Ui::ResultDlg *ui;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // RESULTDLG_H
