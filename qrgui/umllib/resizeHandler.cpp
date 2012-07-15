#include "resizeHandler.h"

#include <algorithm>
	
NodeElement::ResizeHandler::ResizeHandler(
		NodeElement* const resizingNode
		, ElementImpl* const elementImpl
		)
	: mResizingNode(resizingNode)
	, mElementImpl(elementImpl)
{
}

void NodeElement::ResizeHandler::resize(QRectF newContents, QPointF newPos) const
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

	if (SettingsManager::value("ActivateGrid").toBool()) {
		mResizingNode->alignToGrid();
	}
}

qreal NodeElement::ResizeHandler::maxChildWidth() const
{
	qreal maxChildWidthValue = 0;

	foreach (const QGraphicsItem* const childItem, mResizingNode->childItems()) {
		const NodeElement* const curItem = dynamic_cast<const NodeElement* const>(childItem);
		if (!curItem) {
			continue;
		}
		
		maxChildWidthValue = std::max(maxChildWidthValue, curItem->mContents.width());
	}
	if (maxChildWidthValue == 0) {
		maxChildWidthValue = mResizingNode->childrenBoundingRect().width();
	}

	return maxChildWidthValue;
}

void NodeElement::ResizeHandler::sortChildrenIfNeeded() const
{
	if (!mElementImpl->isSortingContainer()) {
		return;
	}

	int const sizeOfForestalling = mElementImpl->sizeOfForestalling();
	qreal curChildY = sizeOfForestalling + mTitlePadding;
	qreal maxChildWidthValue = maxChildWidth();

	foreach (QGraphicsItem* const childItem, mResizingNode->childItems()) {
		QGraphicsRectItem* const placeholder = mResizingNode->mPlaceholder;

		if(placeholder != NULL && childItem == placeholder){
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
			: curItem->mContents.width();
		QRectF const rect(sizeOfForestalling, curChildY, necessaryWidth, curItem->mContents.height());

		curItem->setGeometry(rect);
		curItem->storeGeometry();
		curChildY += curItem->mContents.height() + mElementImpl->sizeOfChildrenForestalling() + mChildSpacing;
	}
}

void NodeElement::ResizeHandler::gripeIfMinimizesToChildrenContainer(QRectF& contents) const
{
	if (mElementImpl->minimizesToChildren()) {
		contents = QRectF();
	}
}

void NodeElement::ResizeHandler::parentResizeCall() const {
	NodeElement* const parItem = dynamic_cast<NodeElement* const>(mResizingNode->parentItem());
	if (parItem) {
		parItem->resize();
	}
}

void NodeElement::ResizeHandler::normalizeSize(QRectF& newContents) const {
	if (newContents.width() < objectMinSize) {
		newContents.setWidth(mResizingNode->mFoldedContents.width());
	}

	if (newContents.height() < objectMinSize) {
		newContents.setHeight(mResizingNode->mFoldedContents.height());
	}
}

void NodeElement::ResizeHandler::resizeAccordingToChildren(QRectF& newContents, QPointF& newPos) const {
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
	//newPos += childDeflectionVector;

	expandByChildren(newContents);
}

QPointF NodeElement::ResizeHandler::childDeflection() const
{
	QPointF childDeflectionVector = QPointF(0, 0);
	int const sizeOfForestalling = mElementImpl->sizeOfForestalling();

	foreach (const QGraphicsItem* const childItem, mResizingNode->childItems()) {
		const NodeElement* const curItem = dynamic_cast<const NodeElement* const>(childItem);
		if (!curItem || curItem->isPort()) {
			continue;
		}

		childDeflectionVector.setX(
				std::min(curItem->pos().x() - sizeOfForestalling, childDeflectionVector.x())
				);
		childDeflectionVector.setY(
				std::min(curItem->pos().y() - sizeOfForestalling, childDeflectionVector.y())
				);
	}

	return childDeflectionVector;
}

void NodeElement::ResizeHandler::moveChildren(QPointF const &shift) const
{
	qreal const sizeOfForestalling = mElementImpl->sizeOfForestalling();

	foreach (QGraphicsItem* const childItem, mResizingNode->childItems()) {
		NodeElement* const curItem = dynamic_cast<NodeElement* const>(childItem);
		if (!curItem || !curItem->isPort()) {
			continue;
		}

		curItem->moveBy(shift.x(), shift.y());
	
		QPointF pos(
				std::max(curItem->pos().x(), sizeOfForestalling)
				, std::max(curItem->pos().y(), sizeOfForestalling)
			);
		///returns object to the parent area
		curItem->setPos(pos);
	}
}

void NodeElement::ResizeHandler::expandByChildren(QRectF& contents) const
{
	int const sizeOfForestalling = mElementImpl->sizeOfForestalling();

	foreach (const QGraphicsItem* const childItem, mResizingNode->childItems()) {
		QRectF curChildItemBoundingRect = childBoundingRect(childItem, contents);

		if (
			curChildItemBoundingRect.width() == 0
			||
			curChildItemBoundingRect.height() == 0
		) {
			continue;
		}

		// it seems to be more appropriate to use childItem->pos() but it causes
		// bad behaviour when dropping one element to another
		curChildItemBoundingRect.translate(childItem->scenePos() - mResizingNode->scenePos());

		contents.setLeft(
				std::min(curChildItemBoundingRect.left() - sizeOfForestalling
				       	, contents.left())
				);

		contents.setRight(
				std::max(curChildItemBoundingRect.right() + sizeOfForestalling
				       	, contents.right())
				);

		contents.setTop(
				std::min(curChildItemBoundingRect.top() - sizeOfForestalling
				       	, contents.top())
				);

		contents.setBottom(
				std::max(curChildItemBoundingRect.bottom() + sizeOfForestalling
				       	, contents.bottom())
				);
	}
}

QRectF NodeElement::ResizeHandler::childBoundingRect(const QGraphicsItem* const childItem, QRectF const &contents) const
{
	QRectF boundingRect;

	if (childItem == mResizingNode->mPlaceholder) {
		boundingRect = childItem->boundingRect();

		int const sizeOfForestalling = mElementImpl->sizeOfForestalling();
		boundingRect.setLeft(contents.left() + sizeOfForestalling);
		boundingRect.setRight(contents.right() - sizeOfForestalling);
		
		return boundingRect;
	}

	const NodeElement* const curItem = dynamic_cast<const NodeElement* const>(childItem);
	if (curItem && !curItem->isPort()) {
		boundingRect = curItem->mContents;
	}

	return boundingRect;
}
