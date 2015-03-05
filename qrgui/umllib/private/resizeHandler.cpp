#include "resizeHandler.h"

#include <algorithm>

#include "view/editorViewScene.h"

ResizeHandler::ResizeHandler(NodeElement * const resizingNode)
		: mTargetNode(resizingNode)
		, mElementImpl(resizingNode->elementImpl())
{
}

void ResizeHandler::resize(QRectF newContents, QPointF newPos, bool needResizeParent) const
{
	newContents.moveTo(0, 0);

	sortChildrenIfNeeded();
	gripeIfMinimizesToChildrenContainer(newContents);

	if (!mTargetNode->isFolded()) {
		resizeAccordingToChildren(newContents, newPos);
	}
	normalizeSize(newContents);

	newContents.moveTo(newPos);

	mTargetNode->setGeometry(newContents);
	mTargetNode->storeGeometry();
	mTargetNode->setPos(newPos);

	if (needResizeParent) {
		resizeParent();
	}

	mTargetNode->updateLabels();
}

qreal ResizeHandler::maxChildWidth() const
{
	qreal maxChildWidthValue = 0;
	foreach (const QGraphicsItem * const childItem, mTargetNode->childItems()) {
		const NodeElement * const curItem = dynamic_cast<const NodeElement * const>(childItem);
		if (!curItem) {
			continue;
		}
		maxChildWidthValue = qMax(maxChildWidthValue, curItem->contentsRect().width());
	}
	if (maxChildWidthValue == 0) {
		maxChildWidthValue = mTargetNode->childrenBoundingRect().width();
	}

	return maxChildWidthValue;
}

void ResizeHandler::sortChildrenIfNeeded() const
{
	if (!mElementImpl->isSortingContainer()) {
		return;
	}

	QVector<int> const sizeOfForestalling = mElementImpl->sizeOfForestalling();
	int forestallingTop = sizeOfForestalling[1];
	int forestallingLeft = sizeOfForestalling[0];

	qreal curChildY = forestallingTop;
	qreal const maxChildWidthValue = maxChildWidth();

	QList<NodeElement *> children = sortedChildrenList();
	foreach (QGraphicsItem * const childItem, children) {
		QGraphicsRectItem * const placeholder = mTargetNode->placeholder();

		if(placeholder != NULL && childItem == placeholder) {
			QRectF const rect(forestallingLeft, curChildY, maxChildWidthValue, placeholder->rect().height());
			placeholder->setRect(rect);
			curChildY += placeholder->rect().height();
		}

		NodeElement * const curItem = dynamic_cast<NodeElement* const>(childItem);
		if (!curItem) {
			continue;
		}

		qreal const necessaryWidth =
				mElementImpl->maximizesChildren()
				? maxChildWidthValue
				: curItem->contentsRect().width();
		QRectF const rect(forestallingLeft, curChildY, necessaryWidth, curItem->contentsRect().height());

		curItem->setGeometry(rect);
		curItem->storeGeometry();
		curChildY += curItem->contentsRect().height() + mElementImpl->sizeOfChildrenForestalling();
	}
}

void ResizeHandler::gripeIfMinimizesToChildrenContainer(QRectF &contents) const
{
	if (mElementImpl->minimizesToChildren()) {
		contents = QRectF();
	}
}

void ResizeHandler::resizeParent() const
{
	NodeElement * const parItem = dynamic_cast<NodeElement* const>(mTargetNode->parentItem());
	if (parItem) {
		ResizeHandler const handler(parItem);
		handler.resize(parItem->contentsRect(), parItem->pos());
	}
}

void ResizeHandler::normalizeSize(QRectF &newContents) const
{
	if (newContents.width() < mMinSize) {
		newContents.setWidth(mTargetNode->foldedContentsRect().width());
	}

	if (newContents.height() < mMinSize) {
		newContents.setHeight(mTargetNode->foldedContentsRect().height());
	}
}

void ResizeHandler::resizeAccordingToChildren(QRectF &newContents, QPointF &newPos) const
{
	// Vector of minimum negative XY child deflection from top left corner.
	QPointF const childDeflectionVector = childDeflection();

	moveChildren(-childDeflectionVector);
	newPos += childDeflectionVector;

	newContents.setBottomRight(newContents.bottomRight() - childDeflectionVector);
	expandByChildren(newContents);
}

QPointF ResizeHandler::childDeflection() const
{
	QPointF childDeflectionVector = QPointF(0, 0);
	QVector<int> const sizeOfForestalling = mElementImpl->sizeOfForestalling();
	int forestallingTop = sizeOfForestalling[1];
	int forestallingLeft = sizeOfForestalling[0];

	foreach (const QGraphicsItem * const childItem, mTargetNode->childItems()) {
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
	foreach (const QGraphicsItem * const childItem, mTargetNode->childItems()) {
		const NodeElement * const curItem = dynamic_cast<const NodeElement * const>(childItem);
		if (!curItem) {
			continue;
		}
	}
}

void ResizeHandler::moveChildren(QPointF const &shift) const
{
	QVector<int> const sizeOfForestalling = mElementImpl->sizeOfForestalling();
	qreal forestallingTop = sizeOfForestalling[1];
	qreal forestallingLeft = sizeOfForestalling[0];

	foreach (QGraphicsItem * const childItem, mTargetNode->childItems()) {
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
	QVector<int> const sizeOfForestalling = mElementImpl->sizeOfForestalling();

	foreach (const QGraphicsItem * const childItem, mTargetNode->childItems()) {
		QRectF curChildItemBoundingRect = childBoundingRect(childItem, contents);

		if (curChildItemBoundingRect.width() == 0 || curChildItemBoundingRect.height() == 0) {
			continue;
		}

		// it seems to be more appropriate to use childItem->pos() but it causes
		// bad behaviour when dropping one element to another
		curChildItemBoundingRect.translate(childItem->scenePos() - mTargetNode->scenePos());

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

QRectF ResizeHandler::childBoundingRect(const QGraphicsItem * const childItem, QRectF const &contents) const
{
	QRectF boundingRect;

	if (childItem == mTargetNode->placeholder()) {
		boundingRect = childItem->boundingRect();

		QVector<int> const sizeOfForestalling = mElementImpl->sizeOfForestalling();
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

	IdList childrenIds = mTargetNode->sortedChildren();
	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(mTargetNode->scene());
	foreach (Id const &id, childrenIds) {
		NodeElement *child = evScene->getNodeById(id);
		if (child) {
			result << child;
		}
	}

	return result;
}
