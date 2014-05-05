#include "smileBlock.h"

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel::robotParts;

SmileBlock::SmileBlock(robotModel::parts::TrikDisplay &display)
	: SmileBlockBase(":/icons/trik/smile.png", display)
{
}
