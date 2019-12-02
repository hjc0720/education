#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QWidget>
#include <chrono>

using namespace std::chrono;

namespace Ui {
class ContentWidget;
}

class QLCDNumber;
class ContentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContentWidget(QWidget *parent = 0);
    ~ContentWidget();
public slots:
    void start();
    void setQuestion();
    void showResult();
protected:
    system_clock::time_point    m_startTime;
    int                         m_nSecondsTimer;
    u_char                      m_nId;
    u_char                      m_nAnswerIndex;
    u_char                      m_nRightCount;

private:
    Ui::ContentWidget *ui;

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override;
    int randomRange(int nMin,int nMax);
    void showAnswer(bool bRight);
    QLCDNumber* getSelect(int nIndex);

    // QWidget interface
protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // CONTENTWIDGET_H
