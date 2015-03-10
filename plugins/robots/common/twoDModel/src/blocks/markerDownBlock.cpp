#include "twoDModel/blocks/markerDownBlock.h"

using namespace twoDModel;
using namespace blocks;
using namespace kitBase::robotModel;

MarkerDownBlock::MarkerDownBlock(RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<robotModel::parts::Marker>(robotModel)
{
}

void MarkerDownBlock::doJob(robotModel::parts::Marker &marker)
{
	marker.down(propertyToColor(stringProperty("Color")));
	emit done(mNextBlockId);
}
