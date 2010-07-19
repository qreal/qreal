#pragma once

#include <QDialog>
#include <QtGui>

class CheckoutDialog : public QDialog
{
Q_OBJECT
public:
	CheckoutDialog(QWidget *parent = 0);
	QComboBox *directoryComboBox;

public slots:
	void browse();

private:
	QComboBox *urlComboBox;
	QLabel *urlLabel;
	QLabel *directoryLabel;
	QPushButton *browseButton;
	QPushButton *createButton(const QString &text, const char *member);
	QComboBox *createComboBox(const QString &text = QString());

	QDir currentDir;
};
