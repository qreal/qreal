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
class EdgeElementType : public ElementType
{
public:
	Type type() const override;

	/// Returns the style of the pen that will draw this edge.
	virtual Qt::PenStyle penStyle() const = 0;

	/// Returns the thickness of the pen that will draw this edge.
	virtual int penWidth() const = 0;

	/// Returns the color of the pen that will draw this edge.
	virtual QColor penColor() const = 0;

	/// Returns true if this edge can be divided into two ones by throwing element onto it.
	virtual bool isDividable() const = 0;

	/// Returns a list of port types that can be edges`s begin can be connected to.
	virtual QStringList fromPortTypes() const = 0;

	/// Returns a list of port types that can be edges`s end can be connected to.
	virtual QStringList toPortTypes() const = 0;

	/// Returns default type of this edges`s shape. This shape usially can then be changed by user.
	virtual LinkShape shapeType() const = 0;

	/// Draws with a \painter the arrow from a begin. The arrow type is specified in a metamodel.
	virtual void drawStartArrow(QPainter *painter) const = 0;

	/// Draws with a \painter the arrow from a begin. The arrow type is specified in a metamodel.
	virtual void drawEndArrow(QPainter *painter) const = 0;

protected:
	/// @param metamodel Metamodel that owns this edge element.
	explicit EdgeElementType(Metamodel &metamodel);
};

}
