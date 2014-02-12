#include "scrollArea.h"

using namespace qReal::widgetsEdit;

ScrollArea::ScrollArea(ToolController *controller)
	: AbstractScrollArea(new QScrollArea, controller)
{
	mScrollArea = dynamic_cast<QScrollArea *>(widget());
	mTitle = tr("Scroll Area");
	mTag = "ScrollArea";
	mIcon = QIcon(":/icons/widgetsEditor/scrollArea.png");
	mScrollArea->setGeometry(0, 0
		, SCROLL_AREA_DEFAULT_WIDTH
		, SCROLL_AREA_DEFAULT_WIDTH);
	mProxy = new ScrollAreaProxy(mScrollArea);
}

ScrollAreaProxy::ScrollAreaProxy(QScrollArea *scrollArea)
	: AbstractScrollAreaProxy(scrollArea), mScrollArea(scrollArea)
{
	mScrollArea->setFrameShape(QFrame::StyledPanel);
	mScrollArea->setFrameShadow(QFrame::Sunken);
	mScrollArea->setLineWidth(1);
}

bool ScrollAreaProxy::widgetResizable() const
{
	return mScrollArea->widgetResizable();
}

void ScrollAreaProxy::setWidgetResizable(bool resizable)
{
	mScrollArea->setWidgetResizable(resizable);
}
