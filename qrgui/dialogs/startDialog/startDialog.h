#pragma once
#include <QtGui/QDialog>
#include <QtGui/QTabWidget>

class StartDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit StartDialog(QWidget *parent = 0);

private:
	QTabWidget *mTabWidget;
};
