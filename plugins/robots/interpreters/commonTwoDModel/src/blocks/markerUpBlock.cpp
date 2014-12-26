#include "commonTwoDModel/blocks/markerUpBlock.h"

using namespace twoDModel;
using namespace blocks;
using namespace interpreterBase::robotModel;

MarkerUpBlock::MarkerUpBlock(RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::Marker>(robotModel)
{
}

void MarkerUpBlock::doJob(robotModel::parts::Marker &marker)
{
	marker.up();
	emit done(mNextBlockId);
}
