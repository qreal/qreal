#pragma once

#include <QtWidgets/QApplication>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

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
	void logMouse(QWidget * const target, QMouseEvent * const event);
	void logWheel(QWidget * const target, QWheelEvent * const event);
	void logKey(QKeyEvent * const event);

	void onFocusChanged(QWidget *old, QWidget *now);
};

}
