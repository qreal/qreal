#pragma once;

#include <QApplication>

class QRealApplication : public QApplication
{
	Q_OBJECT
public:
	QRealApplication(int &argc, char **argv);
	bool event(QEvent *event);
};
