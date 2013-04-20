#pragma once

#include <QApplication>
#include <QtGui/QKeyEvent>

class QRealApplication : public QApplication
{
	Q_OBJECT

public:
	QRealApplication(int &argc, char **argv);
	bool notify ( QObject *receiver, QEvent *event);
};
