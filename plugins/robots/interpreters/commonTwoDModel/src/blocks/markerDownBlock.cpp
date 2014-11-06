#include "commonTwoDModel/blocks/markerDownBlock.h"

using namespace twoDModel;
using namespace blocks;
using namespace interpreterBase::robotModel;

MarkerDownBlock::MarkerDownBlock(RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::Marker>(robotModel)
{
}

void MarkerDownBlock::doJob(robotModel::parts::Marker &marker)
{
	marker.down(propertyToColor(stringProperty("Color")));
	emit done(mNextBlockId);
}
