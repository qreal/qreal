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

#include "metaMetaModel/nodeElementType.h"

#include "metaMetaModel/metamodel.h"

using namespace qReal;

NodeElementType::NodeElementType(Metamodel &metamodel)
	: ElementType(metamodel)
	, mIsResizable(false)
	, mIsContainer(false)
	, mIsSortingContainer(false)
	, mSizeOfForestalling({0,0,0,0})
	, mSizeOfChildrenForestalling(0)
	, mHasMovableChildren(false)
	, mMinimizesToChildren(false)
	, mMaximizesChildren(false)
	, mCreateChildrenFromMenu(false)
{
}

ElementType::Type NodeElementType::type() const
{
	return Type::node;
}

QSizeF NodeElementType::size() const
{
	return mSize;
}

void NodeElementType::setSize(const QSizeF &size)
{
	mSize = size;
}

bool NodeElementType::isResizable() const
{
	return mIsResizable;
}

void NodeElementType::setResizable(bool resizable)
{
	mIsResizable = resizable;
}

const QList<qreal> &NodeElementType::border() const
{
	return mBorder;
}

void NodeElementType::setBorder(const QList<qreal> &border)
{
	mBorder = border;
}

const QList<PointPortInfo> &NodeElementType::pointPorts() const
{
	return mPointPorts;
}

void NodeElementType::addPointPort(const PointPortInfo &port)
{
	mPointPorts << port;
	if (!mPortTypes.contains(port.type)) {
		mPortTypes << port.type;
	}
}

const QList<LinePortInfo> &NodeElementType::linePorts() const
{
	return mLinePorts;
}

void NodeElementType::addLinePort(const LinePortInfo &port)
{
	mLinePorts << port;
	if (!mPortTypes.contains(port.type)) {
		mPortTypes << port.type;
	}
}

const QList<CircularPortInfo> &NodeElementType::circularPorts() const
{
	return mCircularPorts;
}

void NodeElementType::addCircularPort(const CircularPortInfo &port)
{
	mCircularPorts << port;
	if (!mPortTypes.contains(port.type)) {
		mPortTypes << port.type;
	}
}

const QStringList &NodeElementType::portTypes() const
{
	return mPortTypes;
}

QString NodeElementType::mouseGesture() const
{
	return mMouseGesture;
}

void NodeElementType::setMouseGesture(const QString &gesture)
{
	mMouseGesture = gesture;
}

bool NodeElementType::isContainer() const
{
	return mIsContainer;
}

void NodeElementType::setContainer(bool isContainer)
{
	mIsContainer = isContainer;
}

bool NodeElementType::isSortingContainer() const
{
	return mIsSortingContainer;
}

void NodeElementType::setSortingContainer(bool isSortingContainer)
{
	mIsSortingContainer = isSortingContainer;
}

const QVector<int> &NodeElementType::sizeOfForestalling() const
{
	return mSizeOfForestalling;
}

void NodeElementType::setSizeOfForestalling(const QVector<int> &margins)
{
	mSizeOfForestalling = margins;
}

int NodeElementType::sizeOfChildrenForestalling() const
{
	return mSizeOfChildrenForestalling;
}

void NodeElementType::setSizeOfChildrenForestalling(int padding)
{
	mSizeOfChildrenForestalling = padding;
}

bool NodeElementType::hasMovableChildren() const
{
	return mHasMovableChildren;
}

void NodeElementType::setChildrenMovable(bool movable)
{
	mHasMovableChildren = movable;
}

bool NodeElementType::minimizesToChildren() const
{
	return mMinimizesToChildren;
}

void NodeElementType::setMinimizesToChildren(bool minimizes)
{
	mMinimizesToChildren = minimizes;
}

bool NodeElementType::maximizesChildren() const
{
	return mMaximizesChildren;
}

void NodeElementType::setMaximizesChildren(bool maximizes)
{
	mMaximizesChildren = maximizes;
}

bool NodeElementType::createChildrenFromMenu() const
{
	return mCreateChildrenFromMenu;
}

void NodeElementType::setCreateChildrenFromMenu(bool canCreate)
{
	mCreateChildrenFromMenu = canCreate;
}
