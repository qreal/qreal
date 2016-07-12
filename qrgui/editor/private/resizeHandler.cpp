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

#include "resizeHandler.h"

#include <algorithm>
#include <metaMetaModel/nodeElementType.h>

#include "editor/editorViewScene.h"

using namespace qReal::gui::editor;

ResizeHandler::ResizeHandler(NodeElement &resizingNode)
	: mTargetNode(resizingNode)
	, mElementType(resizingNode.nodeType())
{
}

void ResizeHandler::resize(QRectF newContents, QPointF newPos, bool needResizeParent) const
{
	newContents.moveTo(0, 0);

	sortChildrenIfNeeded();
	gripeIfMinimizesToChildrenContainer(newContents);

	if (!mTargetNode.isFolded()) {
		resizeAccordingToChildren(newContents, newPos);
	}
	normalizeSize(newContents);

	newContents.moveTo(newPos);

	mTargetNode.setGeometry(newContents);
	mTargetNode.storeGeometry();
	mTargetNode.setPos(newPos);

	if (needResizeParent) {
		resizeParent();
	}

	mTargetNode.updateLabels();
}

qreal ResizeHandler::maxChildWidth() const
{
	qreal maxChildWidthValue = 0;
	for (const QGraphicsItem * const childItem : mTargetNode.childItems()) {
		const NodeElement * const curItem = dynamic_cast<const NodeElement * const>(childItem);
		if (!curItem) {
			continue;
		}
		maxChildWidthValue = qMax(maxChildWidthValue, curItem->contentsRect().width());
	}

	if (maxChildWidthValue == 0) {
		maxChildWidthValue = mTargetNode.childrenBoundingRect().width();
	}

	return maxChildWidthValue;
}

void ResizeHandler::sortChildrenIfNeeded() const
{
	if (!mElementType.isSortingContainer()) {
		return;
	}

	const QVector<int> sizeOfForestalling = mElementType.sizeOfForestalling();
	int forestallingTop = sizeOfForestalling[1];
	int forestallingLeft = sizeOfForestalling[0];

	qreal curChildY = forestallingTop;
	const qreal maxChildWidthValue = maxChildWidth();

	QList<NodeElement *> children = sortedChildrenList();
	for (QGraphicsItem * const childItem : children) {
		QGraphicsRectItem * const placeholder = mTargetNode.placeholder();

		if(placeholder != nullptr && childItem == placeholder) {
			const QRectF rect(forestallingLeft, curChildY, maxChildWidthValue, placeholder->rect().height());
			placeholder->setRect(rect);
			curChildY += placeholder->rect().height();
		}

		NodeElement * const curItem = dynamic_cast<NodeElement* const>(childItem);
		if (!curItem) {
			continue;
		}

		const qreal necessaryWidth =
				mElementType.maximizesChildren()
				? maxChildWidthValue
				: curItem->contentsRect().width();
		const QRectF rect(forestallingLeft, curChildY, necessaryWidth, curItem->contentsRect().height());

		curItem->setGeometry(rect);
		curItem->storeGeometry();
		curChildY += curItem->contentsRect().height() + mElementType.sizeOfChildrenForestalling();
	}
}

void ResizeHandler::gripeIfMinimizesToChildrenContainer(QRectF &contents) const
{
	if (mElementType.minimizesToChildren()) {
		contents = QRectF();
	}
}

void ResizeHandler::resizeParent() const
{
	NodeElement * const parItem = dynamic_cast<NodeElement* const>(mTargetNode.parentItem());
	if (parItem) {
		const ResizeHandler handler(*parItem);
		handler.resize(parItem->contentsRect(), parItem->pos());
	}
}

void ResizeHandler::normalizeSize(QRectF &newContents) const
{
	if (newContents.width() < mMinSize) {
		newContents.setWidth(mTargetNode.foldedContentsRect().width());
	}

	if (newContents.height() < mMinSize) {
		newContents.setHeight(mTargetNode.foldedContentsRect().height());
	}
}

void ResizeHandler::resizeAccordingToChildren(QRectF &newContents, QPointF &newPos) const
{
	// Vector of minimum negative XY child deflection from top left corner.
	const QPointF childDeflectionVector = childDeflection();

	moveChildren(-childDeflectionVector);
	newPos += childDeflectionVector;

	newContents.setBottomRight(newContents.bottomRight() - childDeflectionVector);
	expandByChildren(newContents);
}

QPointF ResizeHandler::childDeflection() const
{
	QPointF childDeflectionVector = QPointF(0, 0);
	QVector<int> const sizeOfForestalling = mElementType.sizeOfForestalling();
	int forestallingTop = sizeOfForestalling[1];
	int forestallingLeft = sizeOfForestalling[0];

	for (const QGraphicsItem * const childItem : mTargetNode.childItems()) {
		const NodeElement * const curItem = dynamic_cast<const NodeElement * const>(childItem);
		if (!curItem) {
			continue;
		}

		childDeflectionVector.setX(qMin(curItem->pos().x() - forestallingLeft, childDeflectionVector.x()));
		childDeflectionVector.setY(qMin(curItem->pos().y() - forestallingTop, childDeflectionVector.y()));
	}

	return childDeflectionVector;
}

void ResizeHandler::printChildPos() const
{
	for (const QGraphicsItem * const childItem : mTargetNode.childItems()) {
		const NodeElement * const curItem = dynamic_cast<const NodeElement * const>(childItem);
		if (!curItem) {
			continue;
		}
	}
}

void ResizeHandler::moveChildren(const QPointF &shift) const
{
	QVector<int> const sizeOfForestalling = mElementType.sizeOfForestalling();
	qreal forestallingTop = sizeOfForestalling[1];
	qreal forestallingLeft = sizeOfForestalling[0];

	for (QGraphicsItem * const childItem : mTargetNode.childItems()) {
		NodeElement * const curItem = dynamic_cast<NodeElement * const>(childItem);
		if (!curItem) {
			continue;
		}

		curItem->moveBy(shift.x(), shift.y());

		QPointF pos(qMax(curItem->pos().x(), forestallingLeft)
				, qMax(curItem->pos().y(), forestallingTop));
		// returns object to the parent area
		curItem->setPos(pos);
	}
}

void ResizeHandler::expandByChildren(QRectF &contents) const
{
	QVector<int> const sizeOfForestalling = mElementType.sizeOfForestalling();

	for (const QGraphicsItem * const childItem : mTargetNode.childItems()) {
		QRectF curChildItemBoundingRect = childBoundingRect(childItem, contents);

		if (curChildItemBoundingRect.width() == 0 || curChildItemBoundingRect.height() == 0) {
			continue;
		}

		// it seems to be more appropriate to use childItem->pos() but it causes
		// bad behaviour when dropping one element to another
		curChildItemBoundingRect.translate(childItem->scenePos() - mTargetNode.scenePos());

		contents.setLeft(qMin(curChildItemBoundingRect.left() - sizeOfForestalling[0]
						, contents.left()));
		contents.setRight(qMax(curChildItemBoundingRect.right() + sizeOfForestalling[2]
						, contents.right()));
		contents.setTop(qMin(curChildItemBoundingRect.top() - sizeOfForestalling[1]
						, contents.top()));
		contents.setBottom(qMax(curChildItemBoundingRect.bottom() + sizeOfForestalling[3]
						, contents.bottom()));
	}
}

QRectF ResizeHandler::childBoundingRect(const QGraphicsItem * const childItem, const QRectF &contents) const
{
	QRectF boundingRect;

	if (childItem == mTargetNode.placeholder()) {
		boundingRect = childItem->boundingRect();

		QVector<int> const sizeOfForestalling = mElementType.sizeOfForestalling();
		boundingRect.setLeft(contents.left() + sizeOfForestalling[0]);
		boundingRect.setRight(contents.right() - sizeOfForestalling[2]);

		return boundingRect;
	}

	const NodeElement * const curItem = dynamic_cast<const NodeElement * const>(childItem);
	if (curItem) {
		boundingRect = curItem->contentsRect();
	}

	return boundingRect;
}

QList<NodeElement *> ResizeHandler::sortedChildrenList() const
{
	QList<NodeElement *> result;

	IdList childrenIds = mTargetNode.sortedChildren();
	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(mTargetNode.scene());
	for (const Id &id : childrenIds) {
		NodeElement *child = evScene->getNodeById(id);
		if (child) {
			result << child;
		}
	}

	return result;
}
