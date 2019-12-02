#include "addchardlg.h"
#include "ui_addchardlg.h"
#include <QAudioDeviceInfo>
#include  <QDebug>
#include <QAudioInput>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFile>
#include <QDir>


AddCharDlg::AddCharDlg(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::AddCharDlg),
	m_pFile(nullptr),
	m_pAudio(nullptr)
{
    ui->setupUi(this);
}

AddCharDlg::~AddCharDlg()
{
	delete ui;
}

QString AddCharDlg::getRecordPath(const QString&Chars)
{
	QString filePath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
	if(!QDir::exists(filePath))
		QDir::mkdir(filePath);
	filePath += "/" + Chars + ".raw";
	return filePath;
}

void AddCharDlg::on_start_clicked()
{
	if(ui->text->text().isEmpty())
	{
		QMessageBox::warning(this,tr("没有汉字"),tr("没有汉字,请输入汉字"));
		return;
	}

	if(!m_pAudio)
		startRecord();
	else
		stopRecord();
}

void AddCharDlg::startRecord()
{
	ui->text->setEnabled(false);
	ui->start->setText(tr("停止"));
	QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
	auto format = info.preferredFormat();

	m_pFile = new QFile(getRecordPath(ui->text->text()));
	m_pFile->open( QIODevice::WriteOnly | QIODevice::Truncate);

	m_pAudio = new QAudioInput(format,this);
	m_pAudio->start(m_pFile);
}

void AddCharDlg::stopRecord()
{
	m_pAudio->stop();
	delete m_pAudio;

	m_pFile->close();
	delete m_pFile;

	ui->text->setEnabled(true);
	ui->start->setText(tr("录音"));
}
