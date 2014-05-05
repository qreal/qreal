#include "sadSmileBlock.h"

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel::robotParts;

SadSmileBlock::SadSmileBlock(robotModel::parts::TrikDisplay &display)
	: SmileBlockBase(":/icons/trik/sadSmile.png", display)
{
}
