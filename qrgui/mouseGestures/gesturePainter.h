#pragma once

#include <QtGui/QIconEngine>

#include "qrgui/mouseGestures/mouseGesturesDeclSpec.h"
#include "qrgui/mouseGestures/private/geometricForms.h"

namespace qReal {
namespace gestures {

/// This class get the gesture in QString. Call method pixmap
/// returns the icon as a pixmap with the required size, mode, and state.
/// The default implementation creates a new pixmap and calls paint() to fill it.
class QRGUI_MOUSE_GESTURES_EXPORT GesturePainter : public QIconEngine
{
public:
	GesturePainter(const QString &gesture
			, QColor const &backgroungColor
			, QColor const &gestureColor
			, qreal size);

	void paint(QPainter *painter, QRect const &rect, QIcon::Mode mode, QIcon::State state) override;
	QIconEngine *clone() const override;

private:
	PathVector mGesture;
	QColor mBackgroundColor;
	QColor mGestureColor;
	qreal mSize;
};

}
}
