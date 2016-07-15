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

#include "editor/edgeElement.h"

namespace qReal {
namespace gui {
namespace editor {

/// A factory for creation of specific LineHandler's subclasses.
/// It also groups all the methods that must be changed to support a new LineHandler's subclass.
class LineFactory : public QObject
{
	Q_OBJECT
public:
	LineFactory(EdgeElement *edge);

	/// Create LineHandler according to link type
	LineHandler *createHandler(LinkShape type) const;

	/// Create menu for choosing shape type
	QMenu *shapeTypeMenu() const;

	/// @return String representation of shapeType
	static QString shapeToString(LinkShape shapeType);

	/// @return LinkShape represented by string; return linkShape::unset if string is incorrect
	static LinkShape stringToShape(const QString &string);

private slots:
	void setSquareLine() const;
	void setBrokenLine() const;
	void setCurveLine() const;

private:
	EdgeElement *mEdge; // Doesn't take ownership
};

}
}
}
