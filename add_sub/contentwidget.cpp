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
  ,m_nAnswerIndex(0)
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
	QSettings set("education","add_sub",this);
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
		nStars++;
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
    ui->rightNum->display(m_nRightCount);
    
	int nSignCount = randomRange(1,2);

	std::vector<int> sign;//0:+ 1:-
	std::vector<int> num;
	int ret = 0;
	//first num
	num.push_back(randomRange(0,20));
	sign.push_back(randomRange(0,1));
	//second num;
	if(sign[0])
	{
		num.push_back(randomRange(0,20-num[0]));
		ret = num[0] + num[1];
	}
	else
	{
		num.push_back(randomRange(0,num[0]));
		ret = num[0] - num[1];
	}

	//third num;
	if(nSignCount >=2)
	{
		sign.push_back(randomRange(0,1));
		if(sign[1])
		{
			num.push_back(randomRange(0,20-ret));
			ret = ret + num[2];
		}
		else
		{
			num.push_back(randomRange(0,ret));
			ret = ret - num[2];
		}
	}

	QString qStr = QString::number(num[0]);
	for(int i = 1; i < num.size(); i++)
	{
		qStr += (sign[i - 1] == 1) ? " + " :" - ";
		qStr += QString::number(num[i]);
	}
	qStr += " = ";
	ui->equation->setText(qStr);
    m_nAnswerIndex = randomRange(0,3);

    std::set<int> value;
    while(value.size() != 3)
    {
		int temp = randomRange(0,20);
		if(temp == ret)
            continue;
        value.insert(temp);
    }

    auto it = value.begin();
    for(int i = 0; i < 4; i++)
    {
        if(i == m_nAnswerIndex)
			getSelect(i)->display(ret);
        else
        {
            getSelect(i)->display(*it++);
        }
    }
}


void ContentWidget::mouseReleaseEvent(QMouseEvent *event)
{
    int nIndex = -1;
    for(int i = 0; i < 4; i++)
    {
       if(getSelect(i)->geometry().contains(event->pos()))
       {
           nIndex = i;
           break;
       }
    }
    if(nIndex == -1)
        return;

    showAnswer(nIndex == m_nAnswerIndex);

    if(nIndex == m_nAnswerIndex)
        m_nRightCount++;

    //ui->rightNum->display(m_nRightCount);
    if(m_nId == 10)
        QTimer::singleShot(1000,this,SLOT(showResult()));
    else
        QTimer::singleShot(1000,this,SLOT(setQuestion()));
}
