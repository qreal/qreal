#include "abstractScrollArea.h"

using namespace qReal::widgetsEdit;

AbstractScrollArea::AbstractScrollArea(QAbstractScrollArea *area
		, ToolController *controller)
	: Frame(area, controller)
{
	mAbstractScrollArea = dynamic_cast<QAbstractScrollArea *>(widget());
}

Qt::ScrollBarPolicy AbstractScrollArea::verticalScrollBarPolicy() const
{
	return mAbstractScrollArea->verticalScrollBarPolicy();
}

Qt::ScrollBarPolicy AbstractScrollArea::horizontalScrollBarPolicy() const
{
	return mAbstractScrollArea->horizontalScrollBarPolicy();
}

void AbstractScrollArea::setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
	mAbstractScrollArea->setVerticalScrollBarPolicy(policy);
}

void AbstractScrollArea::setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
	mAbstractScrollArea->setHorizontalScrollBarPolicy(policy);
}
