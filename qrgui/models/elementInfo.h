/* Copyright 2015-2016 Dmitry Mordvinov
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
#include <QtCore/QMap>
#include <QtCore/QDataStream>

#include <qrkernel/ids.h>

namespace qReal {

/// Base for convenient storing node or edge element data for passing it into methods with just one parameter.
class ElementInfo
{
public:
	ElementInfo();
	explicit ElementInfo(bool isEdge);
	ElementInfo(const Id &id
			, const Id &logicalId
			, const Id &logicalParent
			, const Id &graphicalParent
			, const QMap<QString, QVariant> &logicalProperties
			, const QMap<QString, QVariant> &graphicalProperties
			, const Id &explosionTarget = Id()
			, bool isEdge = false);
	virtual ~ElementInfo();

	/// Saves element data into the given byte stream, it can be then read with deserialize().
	/// @returns a reference to \a out for convenient usage.
	virtual QDataStream &serialize(QDataStream &out) const;

	/// Reastores this element state reading it out from the given byte stream written with serialize().
	/// @returns a reference to \a in for convenient usage.
	virtual QDataStream &deserialize(QDataStream &in);

	/// Returns true if \a contains just the same data as this instance.
	virtual bool equals(const ElementInfo &other) const;

	/// Returns true if this instance describes edge, false if node.
	bool isEdge() const;

	/// Returns logical parent id if this element is logical one (id == logicalId) or graphical parent id otherwise.
	Id parent() const;

	/// Returns value stored by key "name" in logical properties.
	QString name() const;

	/// Returns value stored by key "position" in graphical properties.
	QPointF position() const;

	/// Overwrites id of this element with new one (of the same type). Does nothing if current id is empty.
	/// @returns new id.
	Id newId();

	/// Overwrites logical id of this element with new one (of the same type).
	/// If current logical id is empty makes it of the same type as id. If id is empty does nothing.
	/// @returns new logical id.
	Id newLogicalId();

	/// Sets to the \a position graphical property stored by key "position".
	void setPos(const QPointF &position);

	/// An id of this element, can be graphical or logical. In second case id == logicalId must be preserved.
	Id id;

	/// An id of this element in logical model.
	Id logicalId;

	/// Logical parent id of this element. May be empty if graphical instance is described.
	Id logicalParent;

	/// Graphical parent id of this element. May be empty if logical instance is described.
	Id graphicalParent;

	/// Known properties of logical instance of the element. Must have "name" property.
	QMap<QString, QVariant> logicalProperties;

	/// Known properties of graphical instance of the element. Can have position property setted explicitly.
	QMap<QString, QVariant> graphicalProperties;

	/// Logical id of element that opened by editor when this element is double-clicked.
	Id explosionTarget;

private:
	bool mIsEdge;
};

}

bool operator== (const qReal::ElementInfo &first, const qReal::ElementInfo &second);
