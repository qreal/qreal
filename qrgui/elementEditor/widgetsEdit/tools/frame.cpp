#include "frame.h"

using namespace qReal::widgetsEdit;

Frame::Frame(ToolController *controller)
	: LayoutTool(new QFrame, controller)
{
	initialize();
	mFrame->setFrameShape(QFrame::Panel);
	mFrame->setFrameShadow(QFrame::Raised);
	mProxy = new FrameProxy(mFrame);
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
	mTitle = tr("Frame");
	mTag = "Frame";
	mIcon = QIcon(":/icons/widgetsEditor/frame.png");
}

FrameProxy::FrameProxy(QFrame *frame)
	: LayoutToolProxy(frame), mFrame(frame)
{
	mFrame->setGeometry(0, 0
		, FRAME_DEFAULT_WIDTH
		, FRAME_DEFAULT_HEIGHT);
	mFrame->setMinimumWidth(FRAME_MINIMAL_WIDTH);
	mFrame->setMinimumHeight(FRAME_MINIMAL_HEIGHT);
}

QFrame::Shape FrameProxy::frameShape() const
{
	return mFrame->frameShape();
}

QFrame::Shadow FrameProxy::frameShadow() const
{
	return mFrame->frameShadow();
}

int FrameProxy::lineWidth() const
{
	return mFrame->lineWidth();
}

int FrameProxy::midLineWidth() const
{
	return mFrame->midLineWidth();
}

void FrameProxy::setFrameShape(QFrame::Shape shape)
{
	mFrame->setFrameShape(shape);
}

void FrameProxy::setFrameShadow(QFrame::Shadow shadow)
{
	mFrame->setFrameShadow(shadow);
}

void FrameProxy::setLineWidth(int width)
{
	mFrame->setLineWidth(width);
}

void FrameProxy::setMidLineWidth(int width)
{
	mFrame->setMidLineWidth(width);
}
