/* Copyright 2016 Dmitry Mordvinov
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

#include "metaMetaModel/elementType.h"
#include "metaMetaModel/linkShape.h"

class QPainter;

namespace qReal {

/// Describes type of edge elements that can be instantiated on scene.
class QRGUI_META_META_MODEL_EXPORT EdgeElementType : public ElementType
{
public:
	/// @param metamodel Metamodel that owns this edge element.
	explicit EdgeElementType(Metamodel &metamodel);

	Type type() const override;

	/// Returns the style of the pen that will draw this edge.
	Qt::PenStyle penStyle() const;

	/// Sets the style of the pen that will draw this edge.
	void setPenStyle(Qt::PenStyle style);

	/// Returns the thickness of the pen that will draw this edge.
	int penWidth() const;

	/// Sets the thickness of the pen that will draw this edge.
	void setPenWidth(int width);

	/// Returns the color of the pen that will draw this edge.
	QColor penColor() const;

	/// Sets the color of the pen that will draw this edge.
	void setPenColor(const QColor &color);

	/// Returns true if this edge can be divided into two ones by throwing element onto it.
	bool isDividable() const;

	/// Switches ability to divide this edge into two ones by throwing element onto it.
	void setDividable(bool isDividable);

	/// Returns a list of port types that can be edges`s begin can be connected to.
	const QStringList &fromPortTypes() const;

	/// Sets a list of port types that can be edges`s begin can be connected to.
	void setFromPortTypes(const QStringList &portTypes);

	/// Returns a list of port types that can be edges`s end can be connected to.
	const QStringList &toPortTypes() const;

	/// Sets a list of port types that can be edges`s end can be connected to.
	void setToPortTypes(const QStringList &portTypes);

	/// Returns default type of this edges`s shape. This shape usually can then be changed by user.
	LinkShape shapeType() const;

	/// Sets the default type of this edges`s shape. This shape usually can then be changed by user.
	void setShapeType(LinkShape shape);

	/// Draws with a \painter the arrow from a begin. The arrow type is specified in a metamodel.
	/// @todo: This should be a part of an engine!
	virtual void drawStartArrow(QPainter *painter) const;

	/// Draws with a \painter the arrow from a begin. The arrow type is specified in a metamodel.
	/// @todo: This should be a part of an engine!
	virtual void drawEndArrow(QPainter *painter) const;

private:
	void updateSdf();

	Qt::PenStyle mPenStyle;
	int mPenWidth;
	QColor mPenColor;
	bool mIsDividable;
	QStringList mFromPortTypes;
	QStringList mToPortTypes;
	LinkShape mShapeType;
};

}
