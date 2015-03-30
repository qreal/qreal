#include "detectLineBlock.h"

using namespace trik::blocks::details;

DetectLineBlock::DetectLineBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<trik::robotModel::parts::TrikLineSensor>(robotModel)
{
}

void DetectLineBlock::doJob(trik::robotModel::parts::TrikLineSensor &camera)
{
	camera.detectLine();
	emit done(mNextBlockId);
}
