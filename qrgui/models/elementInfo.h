/* Copyright 2015 Dmitry Mordvinov
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

#include <QtCore/QPointF>

#include <qrkernel/ids.h>

namespace qReal {

/// Container for convenient storing node or edge element data for passing it into methods with just one parameter.
class ElementInfo
{
public:
	/// @param id Graphical or logical id of element.
	/// @param id Logical id of element. May be equal to \a id, then this instance describes logical part of element.
	/// @param parent Graphical or logical id of parent, depending on \a id == \a logicalId.
	/// @param name Element name, if not specified \a id.element() will be used by system.
	/// @param position Position of element on the scene. Actual only for graphical instances.
	ElementInfo(const Id &id
			, const Id &logicalId
			, const Id &parent
			, const QString &name
			, const QPointF &position);

	/// @returns graphical or logical id of element.
	Id id() const;

	/// @returns logical id of element. May be equal to \a id, then this instance describes logical part of element.
	Id logicalId() const;

	/// @returns graphical or logical id of parent, depending on \a id == \a logicalId.
	Id parent() const;

	/// @returns element name, may be empty.
	QString name() const;

	/// @returns position of element on the scene. Actual only for graphical instances.
	QPointF position() const;

	/// Replaces logical id with given and returns a reference to itself.
	ElementInfo &withLogicalId(const Id &logicalId);

private:
	Id mId;
	Id mLogicalId;
	Id mParent;
	QString mName;
	QPointF mPosition;
};

}
