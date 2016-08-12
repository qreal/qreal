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

#include <QtCore/QSizeF>

#include "metaMetaModel/elementType.h"
#include "metaMetaModel/portHelpers.h"

namespace qReal {

/// Describes type of node elements that can be instantiated on scene.
class QRGUI_META_META_MODEL_EXPORT NodeElementType : public ElementType
{
public:
	/// @param metamodel Metamodel that owns this node element.
	explicit NodeElementType(Metamodel &metamodel);

	Type type() const override;

	/// Returns size of the node element`s appearance.
	QSizeF size() const;

	/// Sets size of the node element`s appearance.
	void setSize(const QSizeF &size);

	/// Returns true if instances of this type can be resized with mouse by user.
	/// If true size grip will appear in element`s corner(s).
	bool isResizable() const;

	/// Enables or disables ablilty to resized by mouse instances of this type.
	/// If true size grip will appear in element`s corner(s).
	void setResizable(bool resizable);

	/// Returns the border points of this element.
	const QList<qreal> &border() const;

	/// Sets the border points of this element.
	void setBorder(const QList<qreal> &border);

	/// Returns a list of points to which edges can be with this instances of this type.
	const QList<PointPortInfo> &pointPorts() const;

	/// Appends \a port to a list of points to which edges can be with this instances of this type.
	void addPointPort(const PointPortInfo &port);

	/// Returns a list of segments to which edges can be with this instances of this type.
	const QList<LinePortInfo> &linePorts() const;

	/// Appends \a port to a list of segments to which edges can be with this instances of this type.
	void addLinePort(const LinePortInfo &port);

	/// Returns a list of ellipses to which edges can be with this instances of this type.
	const QList<CircularPortInfo> &circularPorts() const;

	/// Appends \a port to a list of ellipses to which edges can be with this instances of this type.
	void addCircularPort(const CircularPortInfo &port);

	/// Returns a list of types of all ports on the graphical representation of this element.
	const QStringList &portTypes() const;

	/// Returns a serialized mouse gesture for this element.
	QString mouseGesture() const;

	/// Sets a serialized mouse gesture for this element.
	void setMouseGesture(const QString &gesture);

	// --- Container properties. Maybe moving it into new type ContaineElementType has sense? ---

	/// Returns true if this node is a container.
	bool isContainer() const;

	/// Switches ability of this node to contain other elements.
	void setContainer(bool isContainer);

	/// Returns true if this node has vertical layout.
	/// Has sense only when isContainer() returns true.
	/// @todo: Rewrite this shit with QGraphicsLayouts.
	bool isSortingContainer() const ;

	/// Turns on or off vertical layout of chidren of this element.
	/// Has sense only when isContainer() returns true.
	/// @todo: Rewrite this shit with QGraphicsLayouts.
	void setSortingContainer(bool isSortingContainer);

	/// Returns a margins of child elements vertical layout.
	/// Has sense only when isContainer() returns true.
	/// @todo: Rewrite this shit with QGraphicsLayouts.
	const QVector<int> &sizeOfForestalling() const;

	/// Sets a margins of child elements vertical layout.
	/// Has sense only when isContainer() returns true.
	/// @todo: Rewrite this shit with QGraphicsLayouts.
	void setSizeOfForestalling(const QVector<int> &margins);

	/// Returns a padding of child elements vertical layout.
	/// Has sense only when isContainer() returns true.
	/// @todo: Rewrite this shit with QGraphicsLayouts.
	int sizeOfChildrenForestalling() const;

	/// Sets a padding of child elements vertical layout.
	/// Has sense only when isContainer() returns true.
	/// @todo: Rewrite this shit with QGraphicsLayouts.
	void setSizeOfChildrenForestalling(int padding);

	/// Returns true if children can be dragged inside and out of this container.
	/// Has sense only when isContainer() returns true.
	bool hasMovableChildren() const;

	/// Switches the ability to drag children inside and out of this container.
	/// Has sense only when isContainer() returns true.
	void setChildrenMovable(bool movable);

	/// Returns true if this container automaticly shrinks its size to children space.
	/// Has sense only when isContainer() returns true.
	bool minimizesToChildren() const;

	/// Switches automatical shrinking container`s size to children space.
	/// Has sense only when isContainer() returns true.
	void setMinimizesToChildren(bool minimizes);

	/// Returns true if this container automaticly increments size of its children to fit container size growth.
	/// Has sense only when isContainer() returns true.
	bool maximizesChildren() const;

	/// Switches automatical incrementing container`s children size to fit container size growth.
	/// Has sense only when isContainer() returns true.
	void setMaximizesChildren(bool maximizes);

	/// If true is returned container types will let create children inside of them from context menu.
	/// Has sense only when isContainer() returns true.
	/// @todo: Why do we need this shit? Think we must get rid of it.
	bool createChildrenFromMenu() const;

	/// Switches the ability to create children inside of container from context menu.
	/// Has sense only when isContainer() returns true.
	/// @todo: Why do we need this shit? Think we must get rid of it.
	void setCreateChildrenFromMenu(bool canCreate);

private:
	QSizeF mSize;
	bool mIsResizable;
	QList<qreal> mBorder;
	QList<PointPortInfo> mPointPorts;
	QList<LinePortInfo> mLinePorts;
	QList<CircularPortInfo> mCircularPorts;
	QStringList mPortTypes;
	QString mMouseGesture;

	bool mIsContainer;
	bool mIsSortingContainer;
	QVector<int> mSizeOfForestalling;
	int mSizeOfChildrenForestalling;
	bool mHasMovableChildren;
	bool mMinimizesToChildren;
	bool mMaximizesChildren;
	bool mCreateChildrenFromMenu;
};

}
