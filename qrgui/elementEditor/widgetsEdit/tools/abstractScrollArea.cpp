#include "abstractScrollArea.h"

using namespace qReal::widgetsEdit;

AbstractScrollArea::AbstractScrollArea(QAbstractScrollArea *area
		, ToolController *controller)
	: Frame(area, controller)
{
	mAbstractScrollArea = dynamic_cast<QAbstractScrollArea *>(widget());
}

AbstractScrollAreaProxy::AbstractScrollAreaProxy(QAbstractScrollArea *area)
	: FrameProxy(area), mAbstractScrollArea(area)
{
}

Qt::ScrollBarPolicy AbstractScrollAreaProxy::verticalScrollBarPolicy() const
{
	return mAbstractScrollArea->verticalScrollBarPolicy();
}

Qt::ScrollBarPolicy AbstractScrollAreaProxy::horizontalScrollBarPolicy() const
{
	return mAbstractScrollArea->horizontalScrollBarPolicy();
}

void AbstractScrollAreaProxy::setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
	mAbstractScrollArea->setVerticalScrollBarPolicy(policy);
}

void AbstractScrollAreaProxy::setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
	mAbstractScrollArea->setHorizontalScrollBarPolicy(policy);
}
