#pragma once

#include <QDialog>
#include <QtGui>

class CheckoutDialog : public QDialog
{
Q_OBJECT
public:
	CheckoutDialog(QWidget *parent = 0);
	QString getUrl();
	QString getDir();

public slots:
	void browse();

private:
	QComboBox *urlComboBox;
	QComboBox *directoryComboBox;
	QLabel *urlLabel;
	QLabel *directoryLabel;
	QPushButton *browseButton;
	QPushButton *ok;
	QPushButton *cancel;
	QPushButton *createButton(const QString &text, const char *member);
	QComboBox *createComboBox(const QString &text = QString());

	QDir currentDir;
};
