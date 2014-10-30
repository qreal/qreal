#include "commonTwoDModel/blocks/removeMarkerBlock.h"

using namespace twoDModel;
using namespace blocks;
using namespace interpreterBase::robotModel;

RemoveMarkerBlock::RemoveMarkerBlock(RobotModelInterface &robotModel)
	: interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::Marker>(robotModel)
{
}

void RemoveMarkerBlock::doJob(robotModel::parts::Marker &marker)
{
	marker.remove();
	emit done(mNextBlockId);
}
