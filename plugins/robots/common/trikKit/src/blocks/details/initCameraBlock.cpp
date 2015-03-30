#include "initCameraBlock.h"

using namespace trik::blocks::details;

InitCameraBlock::InitCameraBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<trik::robotModel::parts::TrikLineSensor>(robotModel)
{
}

void InitCameraBlock::doJob(trik::robotModel::parts::TrikLineSensor &camera)
{
	camera.init();
	emit done(mNextBlockId);
}
