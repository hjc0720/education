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
    QWidget(parent)
    ,m_nTotalSeconds(60)
  ,m_nAnswerIndex(0)
    ,ui(new Ui::ContentWidget)
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
    m_nSecondsTimer =  startTimer(1000,Qt::PreciseTimer);
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
    QSettings set("education","add_sub_time",this);

    int preCount = set.value("rigthcount",40).toInt();

    if(m_nRightCount > preCount)
        set.setValue("rightcount",m_nRightCount);

    int nCount = set.value("count",0).toInt();

    QString key(QString("Test%1").arg(nCount));

    system_clock::time_point curTime =  system_clock::now();
    time_t tt = system_clock::to_time_t(curTime);
    set.setValue(key+"/date",ctime(&tt));
    set.setValue(key+"/right",m_nRightCount);

    int nStars = 0;
    int nMinCount = std::min(59,std::max(preCount,40));
    int nMaxCount = std::max(preCount,60);
    if(m_nRightCount >= nMinCount)
    {
        nStars = 4*(m_nRightCount - nMinCount) / (nMaxCount - nMinCount)+1;
    }
    set.setValue(key+"/star",nStars);
    set.setValue("count",++nCount);

    ResultDlg dlg(this);
    dlg.setRightCount(m_nRightCount);
    dlg.setStars(nStars);;
    dlg.setUseTime(m_nTotalSeconds);
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
    int nLeftSeconds = m_nTotalSeconds - workTime.count();
    if(nLeftSeconds < 10 && nLeftSeconds >=0)
        ui->workTime->setStyleSheet("color:red");
    else if(nLeftSeconds <=0)
    {
        killTimer(m_nSecondsTimer);
        m_nSecondsTimer = 0;
        showResult();
    }
    else
        ui->workTime->setStyleSheet("color:black");
    ui->workTime->display(static_cast<int>(nLeftSeconds));
}

void ContentWidget::setQuestion()
{
    getSelect(m_nAnswerIndex)->setStyleSheet(QString(""));
    m_nId++;
    ui->id->display(m_nId);
    ui->rightNum->display(m_nRightCount);
    
    int nSignCount = 1;

	std::vector<int> sign;//0:+ 1:-
	std::vector<int> num;
	int ret = 0;
	//first num
    num.push_back(randomRange(0,10));
	sign.push_back(randomRange(0,1));
	//second num;
	if(sign[0])
	{
        num.push_back(randomRange(0,10-num[0]));
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
    for(int i = 1; i < (int)num.size(); i++)
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
        int temp = randomRange(0,10);
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
    {
        m_nRightCount++;
        setQuestion();
    }
    else
    {
        QTimer::singleShot(1000,this,SLOT(setQuestion()));
    }

    //ui->rightNum->display(m_nRightCount);
//    if(m_nId == 10)
//        QTimer::singleShot(1000,this,SLOT(showResult()));
//    else
//        QTimer::singleShot(1000,this,SLOT(setQuestion()));
}
