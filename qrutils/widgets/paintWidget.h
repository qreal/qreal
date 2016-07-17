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

#include <QtWidgets/QWidget>

#include <qrutils/widgets/painterInterface.h>
#include <qrutils/utilsDeclSpec.h>

namespace qReal {
namespace ui {

/// A widget that can be used for separating painting logic from the widget itself.
/// Accepts a number of PainterInterface objects and redraws itself calling paint() method of each painter.
/// @see qReal::ui::PainterInterface.
class QRUTILS_EXPORT PaintWidget : public QWidget
{
	Q_OBJECT

public:
	explicit PaintWidget(QWidget *parent = 0);

	/// Sets painter which will be used to draw.
	void setPainter(PainterInterface *painter);

protected:
	void paintEvent(QPaintEvent *event);

private:
	PainterInterface* mPainter;  // doesn't have ownership
};

}
}
