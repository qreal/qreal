#include "commonTwoDModel/blocks/prepareMarkerBlock.h"

using namespace twoDModel;
using namespace blocks;
using namespace interpreterBase::robotModel;

PrepareMarkerBlock::PrepareMarkerBlock(RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::Marker>(robotModel)
{
}

void PrepareMarkerBlock::doJob(robotModel::parts::Marker &marker)
{
	marker.prepare(propertyToColor(stringProperty("Color")));
	emit done(mNextBlockId);
}
