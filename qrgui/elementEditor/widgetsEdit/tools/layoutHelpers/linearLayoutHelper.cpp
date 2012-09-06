#include <QtGui/QWidget>
#include <QtGui/QPainter>

#include "linearLayoutHelper.h"

using namespace qReal::widgetsEdit;

LinearLayoutHelper::LinearLayoutHelper(LayoutTool *tool
		, Qt::Orientation const orientation)
	: LayoutHelperBase(tool)
	, mTool(tool)
	, mOrientation(orientation)
{
}

void LinearLayoutHelper::drawCurrentDropPosition(QPainter *painter)
{
	mCurrentSell = 0;
	int const cellCount = mLayout->count();
	QPointF p1, p2;
	if (Qt::Horizontal == mOrientation) {
		while (mCurrentSell < cellCount &&
				mLayout->itemAt(mCurrentSell)->geometry().left() <= mPos.x()) {
			++mCurrentSell;
		}
		qreal const x = (mCurrentSell < cellCount)
				? mLayout->itemAt(mCurrentSell)->geometry().left()
				: mLayout->geometry().right();
		qreal const y1 = mLayout->geometry().top();
		qreal const y2 = mLayout->geometry().bottom();
		p1 = QPointF(x, y1);
		p2 = QPointF(x, y2);
	} else {
		while (mCurrentSell < cellCount &&
				mLayout->itemAt(mCurrentSell)->geometry().top() <= mPos.y()) {
			++mCurrentSell;
		}
		qreal const y = (mCurrentSell < cellCount)
				? mLayout->itemAt(mCurrentSell)->geometry().top()
				: mLayout->geometry().bottom();
		qreal const x1 = mLayout->geometry().left();
		qreal const x2 = mLayout->geometry().right();
		p1 = QPointF(x1, y);
		p2 = QPointF(x2, y);
	}

	QPen pen;
	pen.setBrush(Qt::blue);
	pen.setWidth(3);
	pen.setCapStyle(Qt::RoundCap);

	painter->save();
	painter->setPen(pen);
	painter->drawLine(p1, p2);
	painter->restore();
}

void LinearLayoutHelper::dropItem(Tool *item)
{
	insertItem(mCurrentSell, item);
	mLayout->activate();
}

void LinearLayoutHelper::resetLayout(QGraphicsLayout *layout)
{
	LayoutHelperBase::resetLayout(layout);

	mTool->setLayout(NULL);
	mLayout = dynamic_cast<QGraphicsLinearLayout *>(layout);
	if (!mLayout || mLayout->orientation() != mOrientation) {
		return;
	}
	foreach (QGraphicsItem *child, mTool->childItems()) {
		Tool *tool = dynamic_cast<Tool *>(child);
		if (!tool) {
			continue;
		}
		insertItem(-1, tool);
	}
	mTool->setLayout(mLayout);
	mLayout->setPreferredSize(mTool->geometry().width(), mTool->geometry().height());
	mTool->setMinimumSize(mLayout->minimumSize());
}

void LinearLayoutHelper::startChildDrag(Tool *item)
{
	mDraggedChild = item;

	int index = 0;
	while (mLayout->itemAt(index) != item) {
		++index;
	}
	mDraggedChildOldSell = index;

	mLayout->removeItem(item);
}

void LinearLayoutHelper::finishChildDrag(bool const success)
{
	if (!success) {
		insertItem(mDraggedChildOldSell, mDraggedChild);
	}
}

void LinearLayoutHelper::generateAttachedProperty(QDomElement &element, Tool *tool)
{
	int index = findTool(tool);
	element.setAttribute("index", index);
}

void LinearLayoutHelper::onToolResized(const QRectF &newGeometry)
{
	mLayout->setPreferredSize(newGeometry.width(), newGeometry.height());
}

void LinearLayoutHelper::insertItem(int index, Tool *item)
{
	mLayout->insertItem(index, item);
	mLayout->setAlignment(item, Qt::AlignCenter);
}

int LinearLayoutHelper::findTool(Tool *tool) const
{
	for (int i = 0; i<mLayout->count(); ++i) {
		if (tool == mLayout->itemAt(i)) {
			return i;
		}
	}
	return -1;
}
