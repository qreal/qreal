#include "frame.h"

using namespace qReal::widgetsEdit;

Frame::Frame(ToolController *controller)
	: LayoutTool(new QFrame, controller)
{
	initialize();
	mFrame->setFrameShape(QFrame::Panel);
	mFrame->setFrameShadow(QFrame::Raised);
}

Frame::Frame(QFrame *frame, ToolController *controller)
	: LayoutTool(frame, controller)
{
	initialize();
	mFrame->setFrameShape(QFrame::NoFrame);
	mFrame->setFrameShadow(QFrame::Plain);
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
}

QFrame::Shape Frame::frameShape() const
{
	return mFrame->frameShape();
}

QFrame::Shadow Frame::frameShadow() const
{
	return mFrame->frameShadow();
}

int Frame::lineWidth() const
{
	return mFrame->lineWidth();
}

int Frame::midLineWidth() const
{
	return mFrame->midLineWidth();
}

void Frame::setFrameShape(QFrame::Shape shape)
{
	mFrame->setFrameShape(shape);
}

void Frame::setFrameShadow(QFrame::Shadow shadow)
{
	mFrame->setFrameShadow(shadow);
}

void Frame::setLineWidth(int width)
{
	mFrame->setLineWidth(width);
}

void Frame::setMidLineWidth(int width)
{
	mFrame->setMidLineWidth(width);
}
