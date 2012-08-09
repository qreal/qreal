#include "resizeHandler.h"

#include <algorithm>

ResizeHandler::ResizeHandler(
		NodeElement* const resizingNode
		, ElementImpl* const elementImpl
		)
	: mResizingNode(resizingNode)
	, mElementImpl(elementImpl)
{
}

void ResizeHandler::resize(QRectF newContents, QPointF newPos) const
{
	newContents.moveTo(0, 0);

	sortChildrenIfNeeded();
	gripeIfMinimizesToChildrenContainer(newContents);

	if (!mResizingNode->isFolded()) {
		resizeAccordingToChildren(newContents, newPos);
	}
	normalizeSize(newContents);

	newContents.moveTo(newPos);

	mResizingNode->setGeometry(newContents);
	mResizingNode->storeGeometry();

	parentResizeCall();
}

qreal ResizeHandler::maxChildWidth() const
{
	qreal maxChildWidthValue = 0;

	foreach (const QGraphicsItem* const childItem, mResizingNode->childItems()) {
		const NodeElement* const curItem = dynamic_cast<const NodeElement* const>(childItem);
		if (!curItem) {
			continue;
		}

		maxChildWidthValue = qMax(maxChildWidthValue, curItem->contentsRect().width());
	}
	if (maxChildWidthValue == 0) {
		maxChildWidthValue = mResizingNode->childrenBoundingRect().width();
	}

	return maxChildWidthValue;
}

void ResizeHandler::sortChildrenIfNeeded() const
{
	if (!mElementImpl->isSortingContainer()) {
		return;
	}

	int const sizeOfForestalling = mElementImpl->sizeOfForestalling();
	qreal curChildY = sizeOfForestalling + mTitlePadding;
	qreal const maxChildWidthValue = maxChildWidth();

	foreach (QGraphicsItem* const childItem, mResizingNode->childItems()) {
		QGraphicsRectItem* const placeholder = mResizingNode->placeholder();

		if(placeholder != NULL && childItem == placeholder) {
			QRectF const rect(sizeOfForestalling, curChildY,
					maxChildWidthValue, placeholder->rect().height());
			placeholder->setRect(rect);
			curChildY += placeholder->rect().height() + mChildSpacing;
		}

		NodeElement* const curItem = dynamic_cast<NodeElement* const>(childItem);
		if (!curItem) {
			continue;
		}

		qreal const necessaryWidth =
				mElementImpl->maximizesChildren()
				? maxChildWidthValue
				: curItem->contentsRect().width();
		QRectF const rect(sizeOfForestalling, curChildY, necessaryWidth, curItem->contentsRect().height());

		curItem->setGeometry(rect);
		curItem->storeGeometry();
		curChildY += curItem->contentsRect().height() + mElementImpl->sizeOfChildrenForestalling() + mChildSpacing;
	}
}

void ResizeHandler::gripeIfMinimizesToChildrenContainer(QRectF& contents) const
{
	if (mElementImpl->minimizesToChildren()) {
		contents = QRectF();
	}
}

void ResizeHandler::parentResizeCall() const
{
	NodeElement* const parItem = dynamic_cast<NodeElement* const>(mResizingNode->parentItem());
	if (parItem) {
		ResizeHandler const handler(parItem, parItem->elementImpl());
		handler.resize(parItem->contentsRect(), parItem->pos());
	}
}

void ResizeHandler::normalizeSize(QRectF& newContents) const
{
	if (newContents.width() < mMinSize) {
		newContents.setWidth(mResizingNode->foldedContentsRect().width());
	}

	if (newContents.height() < mMinSize) {
		newContents.setHeight(mResizingNode->foldedContentsRect().height());
	}
}

void ResizeHandler::resizeAccordingToChildren(QRectF& newContents, QPointF& newPos) const
{
	/*
	* AAAA!!! Who knows why is this code existed????!!!
	*
	foreach (QGraphicsItem *childItem, childItems()) {
		NodeElement* curItem = dynamic_cast<NodeElement*>(childItem);
		if (curItem && curItem->isPort() && newContents != mContents) {
			curItem->resizeChild(newContents, mContents);
		}
	}
	*/

	/// Vector of minimum negative XY child deflection from top left corner.
	QPointF const childDeflectionVector = childDeflection();

	moveChildren(-childDeflectionVector);
	newPos += childDeflectionVector;

	newContents.setBottomRight(newContents.bottomRight() - childDeflectionVector);
	expandByChildren(newContents);
}

QPointF ResizeHandler::childDeflection() const
{
	QPointF childDeflectionVector = QPointF(0, 0);
	int const sizeOfForestalling = mElementImpl->sizeOfForestalling();

	foreach (const QGraphicsItem* const childItem, mResizingNode->childItems()) {
		const NodeElement* const curItem = dynamic_cast<const NodeElement* const>(childItem);
		if (!curItem || curItem->isPort()) {
			continue;
		}

		childDeflectionVector.setX(qMin(curItem->pos().x() - sizeOfForestalling, childDeflectionVector.x()));
		childDeflectionVector.setY(qMin(curItem->pos().y() - sizeOfForestalling, childDeflectionVector.y()));
	}

	return childDeflectionVector;
}

void ResizeHandler::printChildPos() const
{
	foreach (const QGraphicsItem* const childItem, mResizingNode->childItems()) {
		const NodeElement* const curItem = dynamic_cast<const NodeElement* const>(childItem);
		if (!curItem || curItem->isPort()) {
			continue;
		}

		qDebug() << "child pos: " << curItem->pos();
	}
}

void ResizeHandler::moveChildren(QPointF const &shift) const
{
	qreal const sizeOfForestalling = mElementImpl->sizeOfForestalling();

	foreach (QGraphicsItem* const childItem, mResizingNode->childItems()) {
		NodeElement* const curItem = dynamic_cast<NodeElement* const>(childItem);
		if (!curItem || curItem->isPort()) {
			continue;
		}

		curItem->moveBy(shift.x(), shift.y());

		QPointF pos(qMax(curItem->pos().x(), sizeOfForestalling)
				, qMax(curItem->pos().y(), sizeOfForestalling));
		///returns object to the parent area
		curItem->setPos(pos);
	}
}

void ResizeHandler::expandByChildren(QRectF& contents) const
{
	int const sizeOfForestalling = mElementImpl->sizeOfForestalling();

	foreach (const QGraphicsItem* const childItem, mResizingNode->childItems()) {
		QRectF curChildItemBoundingRect = childBoundingRect(childItem, contents);

		if (curChildItemBoundingRect.width() == 0 || curChildItemBoundingRect.height() == 0) {
			continue;
		}

		// it seems to be more appropriate to use childItem->pos() but it causes
		// bad behaviour when dropping one element to another
		curChildItemBoundingRect.translate(childItem->scenePos() - mResizingNode->scenePos());

		contents.setLeft(qMin(curChildItemBoundingRect.left() - sizeOfForestalling
						, contents.left()));
		contents.setRight(qMax(curChildItemBoundingRect.right() + sizeOfForestalling
						, contents.right()));
		contents.setTop(qMin(curChildItemBoundingRect.top() - sizeOfForestalling
						, contents.top()));
		contents.setBottom(qMax(curChildItemBoundingRect.bottom() + sizeOfForestalling
						, contents.bottom()));
	}
}

QRectF ResizeHandler::childBoundingRect(const QGraphicsItem* const childItem, QRectF const &contents) const
{
	QRectF boundingRect;

	if (childItem == mResizingNode->placeholder()) {
		boundingRect = childItem->boundingRect();

		int const sizeOfForestalling = mElementImpl->sizeOfForestalling();
		boundingRect.setLeft(contents.left() + sizeOfForestalling);
		boundingRect.setRight(contents.right() - sizeOfForestalling);

		return boundingRect;
	}

	const NodeElement* const curItem = dynamic_cast<const NodeElement* const>(childItem);
	if (curItem && !curItem->isPort()) {
		boundingRect = curItem->contentsRect();
	}

	return boundingRect;
}
