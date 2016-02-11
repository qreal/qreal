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

#include "modelsDeclSpec.h"

class QMimeData;

namespace qReal {

/// Base for convenient storing node or edge element data for passing it into methods with just one parameter.
class QRGUI_MODELS_EXPORT ElementInfo
{
public:
	ElementInfo();

	explicit ElementInfo(bool isEdge);

	ElementInfo(const Id &id
			, const Id &logicalId
			, const QString &name
			, const Id &explosionTarget
			, bool isEdge);

	ElementInfo(const Id &id
			, const Id &logicalId
			, const Id &logicalParent
			, const Id &graphicalParent
			, const QMap<QString, QVariant> &logicalProperties
			, const QMap<QString, QVariant> &graphicalProperties
			, const Id &explosionTarget
			, bool isEdge);

	virtual ~ElementInfo();

	/// Saves element data into the given byte stream, it can be then read with deserialize().
	/// @returns a reference to \a out for convenient usage.
	virtual QDataStream &serialize(QDataStream &out) const;

	/// Reastores this element state reading it out from the given byte stream written with serialize().
	/// @returns a reference to \a in for convenient usage.
	virtual QDataStream &deserialize(QDataStream &in);

	/// Returns true if \a contains just the same data as this instance.
	virtual bool equals(const ElementInfo &other) const;

	/// Produces and returns mime data object where stored this element.
	/// Transfers ownership to caller.
	QMimeData *mimeData() const;

	/// Extracts ElementInfo instance from mime data, returns it.
	static ElementInfo fromMimeData(const QMimeData *mimeData);

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

	/// Returns target of the explosion (to which expands this element).
	const Id &explosionTarget() const;

	/// Id of this element.
	const Id &id() const;

	/// Logical Id of this element.
	const Id &logicalId() const;

	/// Sets logical Id of this element.
	void setLogicalId(const Id &id);

	/// Logical parent of this element.
	const Id &logicalParent() const;

	/// Sets logical parent of this element.
	void setLogicalParent(const Id &parent);

	/// Graphical parent of this element.
	const Id &graphicalParent() const;

	/// Sets graphical parent of this element.
	void setGraphicalParent(const Id &parent);

	/// List of all logical property names of this element.
	const QList<QString> logicalProperties() const;

	/// Gets logical property with given name.
	QVariant logicalProperty(const QString &propertyName) const;

	/// Sets logical property with given name to given value.
	void setLogicalProperty(const QString &propertyName, const QVariant &propertyValue);

	/// Sets all logical properties of this element to the contents of the given map. All previous properties
	/// are erased.
	void setAllLogicalProperties(const QMap<QString, QVariant> &logicalProperties);

	/// List of all graphical property names of this element.
	const QList<QString> graphicalProperties() const;

	/// Gets graphical property with given name.
	QVariant graphicalProperty(const QString &propertyName) const;

	/// Sets graphical property with given name of the element.
	void setGraphicalProperty(const QString &propertyName, const QVariant &propertyValue);

	/// Sets all graphical properties of this element to the contents of the given map. All previous properties
	/// are erased.
	void setAllGraphicalProperties(const QMap<QString, QVariant> &graphicalProperties);

private:
	/// An id of this element, can be graphical or logical. In second case id == logicalId must be preserved.
	Id mId;

	/// An id of this element in logical model.
	Id mLogicalId;

	/// Logical parent id of this element. May be empty if graphical instance is described.
	Id mLogicalParent;

	/// Graphical parent id of this element. May be empty if logical instance is described.
	Id mGraphicalParent;

	/// Known properties of logical instance of the element. Must have "name" property.
	QMap<QString, QVariant> mLogicalProperties;

	/// Known properties of graphical instance of the element. Can have position property setted explicitly.
	QMap<QString, QVariant> mGraphicalProperties;

	/// Logical id of element that opened by editor when this element is double-clicked.
	Id mExplosionTarget;

	bool mIsEdge = false;
};

QRGUI_MODELS_EXPORT QDataStream &operator<< (QDataStream &out, const qReal::ElementInfo &data);
QRGUI_MODELS_EXPORT QDataStream &operator>> (QDataStream &in, qReal::ElementInfo &data);
QRGUI_MODELS_EXPORT bool operator== (const qReal::ElementInfo &first, const qReal::ElementInfo &second);

}
