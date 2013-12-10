#pragma once

#include <QApplication>

class QRealApplication : public QApplication
{
public:
	QRealApplication(int & argc, char ** argv);

protected:
	bool notify(QObject *, QEvent *);
};
