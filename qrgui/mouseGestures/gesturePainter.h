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
			, const QColor &backgroungColor
			, const QColor &gestureColor
			, qreal size);

	void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;
	QIconEngine *clone() const override;

private:
	PathVector mGesture;
	QColor mBackgroundColor;
	QColor mGestureColor;
	qreal mSize;
};

}
}
