#include "initBtMasterBlock.h"
#include "../robotParts/robotModel.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace qReal::interpreters::robots::details::blocks;

InitBtMasterBlock::InitBtMasterBlock()
{
}

void InitBtMasterBlock::run()
{
}

QString InitBtMasterBlock::name() const
{
	return tr("BtMaster");
}
