#include "subprogramBlock.h"
#include "../tracer.h"

using namespace qReal::interpreters::robots::details::blocks;

SubprogramBlock::SubprogramBlock()
{
}

void SubprogramBlock::run()
{
	Tracer::debug(tracer::enums::blocks, "SubprogramBlock::run", "stepping into "
			+ stringProperty(id(), "name"));
	Id const logicalId = mGraphicalModelApi->logicalId(id());
	Id const logicalDiagram = mLogicalModelApi->logicalRepoApi().outgoingExplosion(logicalId);
	IdList const diagrams = mGraphicalModelApi->graphicalIdsByLogicalId(logicalDiagram);
	if (!diagrams.isEmpty()) {
		emit stepInto(diagrams[0]);
	}
}

void SubprogramBlock::finishedSteppingInto()
{
	Tracer::debug(tracer::enums::blocks, "SubprogramBlock::run", "finished stepping into " + stringProperty(id(), "name"));
	emit done(mNextBlock);
}
