#ifndef __CONNECTION_DIALOG_H__
#define __CONNECTION_DIALOG_H__

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class ConnectionDialog : public QDialog 
{
	Q_OBJECT;

public:
	ConnectionDialog();
	~ConnectionDialog();

signals:
	void dataAccepted(const QString &addr, const int port);

public slots:	
	void acceptData();

private:
	QLabel *addrLabel;
	QLabel *portLabel;
	QLineEdit *addrEdit;
	QLineEdit *portEdit;

	QPushButton *okButton;
	QPushButton *cancelButton;

	QHBoxLayout *lh1;
	QHBoxLayout *lh2;
	QHBoxLayout *lh3;
	QVBoxLayout *lv1;
};




#endif //__CONNECTION_DIALOG_H__
