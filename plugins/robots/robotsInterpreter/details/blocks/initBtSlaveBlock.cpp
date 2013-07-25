#include "initBtSlaveBlock.h"
#include "../robotParts/robotModel.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace qReal::interpreters::robots::details::blocks;

InitBtSlaveBlock::InitBtSlaveBlock()
{
}

void InitBtSlaveBlock::run()
{
}

QString InitBtSlaveBlock::name() const
{
	return tr("BtSlave");
}
