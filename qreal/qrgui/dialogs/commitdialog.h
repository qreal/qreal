#pragma once

#include <QDialog>
#include <QtGui>

class CommitDialog : public QDialog
{
Q_OBJECT
public:
	CommitDialog(QWidget *parent = 0);
	QString message();

private:
	QLabel *enterMessageLabel;
	QTextEdit *messageTextEdit;
	QPushButton *ok;
	QPushButton *cancel;
};
