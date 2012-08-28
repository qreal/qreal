#include "abstractScrollArea.h"

using namespace Ui::WidgetsEdit;

AbstractScrollArea::AbstractScrollArea(QAbstractScrollArea *area
		, ToolController *controller)
	: Frame(area, controller)
{
	mAbstractScrollArea = dynamic_cast<QAbstractScrollArea *>(widget());
}
