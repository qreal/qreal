#pragma once

#include <QtWidgets/QApplication>

namespace qReal {

/// Class for QReal Application 
/// for interception all events of application for recording
/// mouse click coordinates.
class QRealApplication : public QApplication
{
public:
	QRealApplication(int & argc, char ** argv);

protected:
	bool notify(QObject *receiver, QEvent *event);

private slots:
	void onFocusChanged(QWidget *old, QWidget *now);
};

}
