#pragma once

#include <QtGui/QTouchEvent>

#include "thirdparty/qsKineticScroller.h"

namespace qReal {
class EditorView;

namespace view {
namespace details {

class TouchSupportManager : public QObject
{
	Q_OBJECT

public:
	explicit TouchSupportManager(EditorView *editorView);

	bool processTouchEvent(QTouchEvent *event);
	bool isGestureRunning() const;

signals:
	void gestureStarted();
	void gestureFinished();

protected:
	virtual bool eventFilter(QObject* object, QEvent* event);

private slots:
	void grabTapAndHold();

private:
	bool handleGesture(QGestureEvent *gestureEvent);
	void processGestureState(QGesture *gesture);
	void handleOneFingerTouch(QTouchEvent *event);

	void simulateMouse(QObject *reciever, QEvent::Type event, QPointF const &pos, Qt::MouseButtons buttons);
	void simulatePress(QTouchEvent *event, Qt::MouseButton button = Qt::LeftButton);
	void simulateMove(QTouchEvent *event);
	void simulateRelease(QTouchEvent *event);
	void simulateDoubleClick(QTouchEvent *event);
	void simulateRightClick(QTapAndHoldGesture *gesture);

	bool isElementUnder(QPointF const &pos);

	EditorView *mEditorView;
	QsKineticScroller mScroller;
	bool mGestureIsRunning;
	int mFingersInGesture;
	Qt::MouseButton mButton;
	qint64 mLastTapTimestamp;
};

}
}
}
