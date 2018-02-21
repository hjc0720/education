#include "contentwidget.h"
#include "ui_contentwidget.h"
#include <random>
#include <set>
#include <QMouseEvent>
#include <QTimer>
#include <QSettings>
#include <algorithm>
#include "resultdlg.h"

ContentWidget::ContentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContentWidget)
{
    ui->setupUi(this);
    m_nSecondsTimer = 0;
}

ContentWidget::~ContentWidget()
{
    killTimer(m_nSecondsTimer);
    delete ui;
}

void ContentWidget::start()
{
    m_nRightCount = 0;
    m_nId = 0;
    m_startTime = system_clock::now();
    if(m_nSecondsTimer != 0)
        killTimer(m_nSecondsTimer);
    m_nSecondsTimer =  startTimer(1000,Qt::VeryCoarseTimer);
    setQuestion();
}

int ContentWidget::randomRange(int nMin, int nMax)
{
   unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
   static std::default_random_engine generator (seed);
   std::uniform_int_distribution<int> distribution(nMin,nMax);
   return distribution(generator);
}

void ContentWidget::showAnswer(bool bRight)
{
    QString style("background-color:");
    style += bRight ? "green;" : "red;";
    getSelect(m_nAnswerIndex)->setStyleSheet(style);
}

void ContentWidget::showResult()
{
    QSettings set("education","add",this);
    int nCount = set.value("count",0).toInt();

    system_clock::time_point curTime =  system_clock::now();
    seconds workTime = duration_cast<seconds>( curTime - m_startTime);
    int preTime = set.value("minTime",999).toInt();
    if(m_nRightCount == 10)
        set.setValue("minTime",std::min<int>(workTime.count(),preTime));

    QString key(QString("Test%1").arg(nCount));
    set.setValue(key+"/time",(int)workTime.count());

    time_t tt = system_clock::to_time_t(curTime);
    set.setValue(key+"/date",ctime(&tt));
    set.setValue(key+"/right",m_nRightCount);

    int nStars = 0;
    if(m_nRightCount == 10)
    {
        //nStars++;
        while(workTime.count() < preTime)
        {
            nStars++;
            preTime /= 2;
        }
    }
    set.setValue(key+"/star",nStars);

    set.setValue("count",++nCount);


    ResultDlg dlg(this);
    dlg.setRightCount(m_nRightCount);
    dlg.setStars(nStars);;
    dlg.setUseTime(workTime.count());
    if(dlg.exec())
    {
        start();
    }
    else
    {
        deleteLater();
    }
}

QLCDNumber *ContentWidget::getSelect(int nIndex)
{
   switch(nIndex)
   {
   case 0:
       return ui->A;
   case 1:
       return ui->B;
   case 2:
       return ui->C;
   case 3:
       return ui->D;
   default:
       return nullptr;
   }
}


void ContentWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() != m_nSecondsTimer)
        return;
    system_clock::time_point curTime =  system_clock::now();
    seconds workTime = duration_cast<seconds>( curTime - m_startTime);
    ui->workTime->display(static_cast<int>(workTime.count()));
}

void ContentWidget::setQuestion()
{
    getSelect(m_nAnswerIndex)->setStyleSheet(QString(""));
    m_nId++;
    ui->id->display(m_nId);
    
    int a = randomRange(1,9);
    int c = randomRange(a+1,10);
    int b = c - a;
    ui->add1->display(a);
    ui->add2->display(b);

    m_nAnswerIndex = randomRange(0,3);

    std::set<int> value;
    while(value.size() != 3)
    {
        int temp = randomRange(1,10);
        if(temp == c)
            continue;
        value.insert(temp);
    }

    auto it = value.begin();
    for(int i = 0; i < 4; i++)
    {
        if(i == m_nAnswerIndex)
            getSelect(i)->display(c);
        else
        {
            getSelect(i)->display(*it++);
        }
    }
}


void ContentWidget::mouseReleaseEvent(QMouseEvent *event)
{
    bool bRight = getSelect(m_nAnswerIndex)->geometry().contains(event->pos());
    showAnswer(bRight);

    if(bRight)
        m_nRightCount++;

    ui->rightNum->display(m_nRightCount);
    if(m_nId == 10)
        QTimer::singleShot(1000,this,SLOT(showResult()));
    else
        QTimer::singleShot(1000,this,SLOT(setQuestion()));
}
