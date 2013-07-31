#pragma once

#include <QtGui/QTouchEvent>

#include "../thirdparty/qsKineticScroller.h"

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

private:

	EditorView *mEditorView;
	QsKineticScroller mScroller;
	bool mGestureIsRunning;
};

}
}
}
