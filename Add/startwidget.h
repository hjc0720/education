#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>

namespace Ui {
class StartWidget;
}

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(QWidget *parent = 0);
    ~StartWidget();
signals:
    void start();
    void history();
private:
    Ui::StartWidget *ui;
};

#endif // STARTWIDGET_H
