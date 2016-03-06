/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtGui/QTouchEvent>

#include "thirdparty/qsKineticScroller.h"

namespace qReal {
namespace gui {
namespace editor {
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

	void simulateMouse(QObject *reciever, QEvent::Type event, const QPointF &pos, Qt::MouseButtons buttons);
	void simulatePress(QTouchEvent *event, Qt::MouseButton button = Qt::LeftButton);
	void simulateMove(QTouchEvent *event);
	void simulateRelease(QTouchEvent *event);
	void simulateDoubleClick(QTouchEvent *event);
	void simulateRightClick(QTapAndHoldGesture *gesture);

	void moveCursor(QTouchEvent *event);

	bool isElementUnder(const QPointF &pos);

	EditorView *mEditorView;
	QsKineticScroller mScroller;
	bool mGestureIsRunning;
	int mFingersInGesture;
	Qt::MouseButton mButton;
	qint64 mLastTapTimestamp;
	QPointF mLastTouchBeginPoint;
};

}
}
}
}
}
