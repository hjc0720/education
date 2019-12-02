#ifndef ADDCHARDLG_H
#define ADDCHARDLG_H

#include <QWidget>

class QAudioInput;
class QFile;

namespace Ui {
class AddCharDlg;
}

class AddCharDlg : public QWidget
{
    Q_OBJECT

public:
    explicit AddCharDlg(QWidget *parent = 0);
    ~AddCharDlg();
public:
	static QString getRecordPath(const QString& Chars);
private slots:
    void on_start_clicked();
protected:
	void startRecord();
	void stopRecord();

protected:
	QAudioInput* m_pAudio;
	QFile*       m_pFile;

private:
    Ui::AddCharDlg *ui;
};

#endif // ADDCHARDLG_H
