#include "scrollArea.h"

using namespace Ui::WidgetsEdit;

ScrollArea::ScrollArea(ToolController *controller)
	: AbstractScrollArea(new QScrollArea, controller)
{
	mScrollArea = dynamic_cast<QScrollArea *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/scrollArea.png");
	mTitle = "Scroll Area";
	mScrollArea->setGeometry(0, 0
		, SCROLL_AREA_DEFAULT_WIDTH
		, SCROLL_AREA_DEFAULT_WIDTH);
	mScrollArea->setFrameShape(QFrame::StyledPanel);
	mScrollArea->setFrameShadow(QFrame::Sunken);
	mScrollArea->setLineWidth(1);
}
