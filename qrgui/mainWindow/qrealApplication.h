#pragma once

#include <QtWidgets/QApplication>

class QMouseEvent;
class QWheelEvent;
class QDropEvent;
class QKeyEvent;

namespace qReal {

/// Class for QReal Application 
/// for interception all events of application for recording
/// mouse click coordinates.
class QRealApplication : public QApplication
{
	Q_OBJECT

public:
	QRealApplication(int & argc, char ** argv);

signals:
	/// This event may be subscribed to obtain all low-level user actions, timer events and everything about
	/// application`s event propagation subsystem. This is very powerful and dangerous tool, use it with care,
	/// it can strongly harm application`s performance.
	void lowLevelEvent(QObject *obj, QEvent *e);

protected:
	bool notify(QObject *receiver, QEvent *event);

private slots:
	void logMouse(QWidget * const target, QMouseEvent * const event);
	void logWheel(QWidget * const target, QWheelEvent * const event);
	void logDrop(QWidget * const target, QDropEvent * const event);
	void logKey(QKeyEvent * const event);

	void onFocusChanged(QWidget *old, QWidget *now);
};

}
