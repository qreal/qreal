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

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtWidgets/QGraphicsItem>

namespace qReal {
namespace gui {
namespace editor {

/// Represents text label on a shape, to be configured and used in editor plugins.
class LabelInterface
{
public:
	virtual ~LabelInterface() {}

	/// @todo: Set properties described in qrgui/editor/labels/labelProperties.cpp class directly through it?

	/// Sets label background.
	virtual void setBackground(const QColor &background) = 0;

	/// Sets label scalability.
	/// @param scalingX If true, label can be resized by X axis.
	/// @param scalingY If true, label can be resized by Y axis.
	virtual void setScaling(bool scalingX, bool scalingY) = 0;

	/// Sets "hardness" of a label.
	/// @param hard If true, label will not be hidden when "text under icons" option is disabled.
	virtual void setHard(bool hard) = 0;

	/// The root of all evil. Method that gets information about label contents and configuration from repository.
	/// @param text Contains current value of the property and its configuration.
	virtual void setTextFromRepo(const QString& text) = 0;

	/// Allows to set graphics item flags for current label.
	virtual void setFlags(QGraphicsItem::GraphicsItemFlags flags) = 0;

	/// Allows to set text interaction flags for current label.
	virtual void setTextInteractionFlags(Qt::TextInteractionFlags flags) = 0;

	/// Sets HTML-formatted text for this label.
	/// @deprecated Use plain text instead.
	virtual void setHtml(const QString &html) = 0;

	/// Sets unformatted text as the contents of this label.
	virtual void setPlainText(const QString &text) = 0;

	/// Sets text drawn just before label contents.
	virtual void setPrefix(const QString &text) = 0;

	/// Sets text drawn just after label contents.
	virtual void setSuffix(const QString &text) = 0;
};

}
}
}
