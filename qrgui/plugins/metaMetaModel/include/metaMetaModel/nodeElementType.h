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
#include "metaMetaModel/portHelpers.h"

namespace qReal {

/// Describes type of node elements that can be instantiated on scene.
class NodeElementType : public ElementType
{
public:
	bool isNode() const override
	{
		return true;
	}

	/// Returns path to file that contains SDF description of element's appearance.
	virtual QString sdfFile() const = 0;

	/// Returns size of the node element`s appearance.
	virtual QSizeF size() const = 0;

	/// Returns true if instances of this type can be resized with mouse by user.
	/// If true size grip will appear in element`s corner(s).
	virtual bool isResizeable() const = 0;

	/// Returns the border points of this element.
	virtual QList<qreal> border() const = 0;

	/// Returns a list of points to which edges can be with this instances of this type.
	virtual QList<PointPortInfo> pointPorts() const = 0;

	/// Returns a list of segments to which edges can be with this instances of this type.
	virtual QList<LinePortInfo> linePorts() const = 0;

	// --- Container properties. Maybe moving it into new type ContaineElementType has sense? ---

	/// Returns true if this node is a container.
	virtual bool isContainer() const = 0;

	/// Returns true if this node has vertical layout.
	/// @todo: Rewrite this shit with QGraphicsLayouts.
	virtual bool isSortingContainer() const = 0;

	/// Returns a margin of child elements vertical layout.
	/// @todo: Rewrite this shit with QGraphicsLayouts.
	virtual QVector<int> sizeOfForestalling() const = 0;

	/// Returns a padding of child elements vertical layout.
	/// @todo: Rewrite this shit with QGraphicsLayouts.
	virtual int sizeOfChildrenForestalling() const = 0;

	/// Returns true if children can be dragged inside and out of this container.
	virtual bool hasMovableChildren() const = 0;

	/// Returns true if this container automaticly shrinks its size to children space.
	virtual bool minimizesToChildren() const = 0;

	/// Returns true if this container automaticly increments size of its children to fit container size growth.
	virtual bool maximizesChildren() const = 0;

	/// If true is returned container types will let create children inside of then from context menu.
	/// @todo: Why do we need this shit? Think we must get rid of it.
	virtual bool createChildrenFromMenu() const = 0;

protected:
	/// @param metamodel Metamodel that owns this node element.
	explicit NodeElementType(Metamodel &metamodel);
};

}
