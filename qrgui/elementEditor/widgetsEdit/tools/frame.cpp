#include "frame.h"

using namespace qReal::widgetsEdit;

Frame::Frame(ToolController *controller)
	: LayoutTool(new QFrame, controller)
{
	initialize();
}

Frame::Frame(QFrame *frame, ToolController *controller)
	: LayoutTool(frame, controller)
{
	initialize();
}

void Frame::initialize()
{
	mFrame = dynamic_cast<QFrame *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/frame.png");
	mTitle = "Frame";
	mFrame->setGeometry(0, 0
		, FRAME_DEFAULT_WIDTH
		, FRAME_DEFAULT_HEIGHT);
	mFrame->setMinimumWidth(FRAME_MINIMAL_WIDTH);
	mFrame->setMinimumHeight(FRAME_MINIMAL_HEIGHT);
	//TODO: Understand why setting enum value here doesn`t work
	// but every other property does (for every property manager)
	mFrame->setFrameShape(QFrame::Box);
}
