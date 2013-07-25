#include "terminateConnectionBlock.h"
#include "../robotParts/robotModel.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace qReal::interpreters::robots::details::blocks;

TerminateConnectionBlock::TerminateConnectionBlock()
{
}

void TerminateConnectionBlock::run()
{
}

QString TerminateConnectionBlock::name() const
{
	return tr("Connection");
}
