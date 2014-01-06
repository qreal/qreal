#pragma once

#include <QtWidgets/QApplication>

/// Class for QReal Application 
/// for interception all events of application for recording
/// mouse click coordinates.
class QRealApplication : public QApplication
{
public:
	QRealApplication(int & argc, char ** argv);

protected:
	bool notify(QObject *receiver, QEvent *event);
};
