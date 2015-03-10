#include "twoDModel/blocks/markerUpBlock.h"

using namespace twoDModel;
using namespace blocks;
using namespace kitBase::robotModel;

MarkerUpBlock::MarkerUpBlock(RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<robotModel::parts::Marker>(robotModel)
{
}

void MarkerUpBlock::doJob(robotModel::parts::Marker &marker)
{
	marker.up();
	emit done(mNextBlockId);
}
