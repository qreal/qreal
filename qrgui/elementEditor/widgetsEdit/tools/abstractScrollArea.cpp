#include "abstractScrollArea.h"

using namespace qReal::widgetsEdit;

AbstractScrollArea::AbstractScrollArea(QAbstractScrollArea *area
		, ToolController *controller)
	: Frame(area, controller)
{
	mAbstractScrollArea = dynamic_cast<QAbstractScrollArea *>(widget());
}
