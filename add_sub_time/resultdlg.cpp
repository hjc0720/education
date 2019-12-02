#include "resultdlg.h"
#include "ui_resultdlg.h"
#include <QPainter>
#include <QPaintEvent>
#include <cmath>

ResultDlg::ResultDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDlg)
{
    ui->setupUi(this);
    m_nStart = 0;

    starPolygon << QPointF(1.0, 0.5);
    for (int i = 1; i < 5; ++i)
        starPolygon << QPointF(0.5 + 0.5 * std::cos(0.8 * i * 3.14),
                               0.5 + 0.5 * std::sin(0.8 * i * 3.14));
}

ResultDlg::~ResultDlg()
{
    delete ui;
}

void ResultDlg::setRightCount(int nCount)
{
    ui->rightCount->display(nCount);
}

void ResultDlg::setUseTime(int nSeconds)
{
    ui->useTime->display(nSeconds);
}

void ResultDlg::setStars(int nStars)
{
    m_nStart = nStars;
    update();
}


void ResultDlg::paintEvent(QPaintEvent *event)
{
    if(m_nStart <= 0)
        return;
    QDialog::paintEvent(event);
    QPainter paint(this);
    QPainter*  painter = &paint;
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);

    painter->setBrush(QBrush(Qt::yellow));
    auto& rect = ui->star->geometry();
    int fscale = std::min(rect.height(),rect.width() / m_nStart);
    int yOffset = (rect.height() - fscale) / 2;
    painter->translate(rect.x(), rect.y() + yOffset);
    painter->scale(fscale, fscale);

    for (int i = 0; i < m_nStart; ++i) {
        painter->drawPolygon(starPolygon, Qt::WindingFill);
        painter->translate(1.0, 0.0);
    }

    painter->restore();
}
