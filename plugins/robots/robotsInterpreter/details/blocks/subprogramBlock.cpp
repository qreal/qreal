#include "subprogramBlock.h"
#include "../tracer.h"
#include "../../../qrutils/nameNormalizer.h"

using namespace qReal::interpreters::robots::details::blocks;

SubprogramBlock::SubprogramBlock()
{
}

void SubprogramBlock::run()
{
	QString const name = stringProperty(id(), "name");
	Tracer::debug(tracer::enums::blocks, "SubprogramBlock::run", "stepping into " + name);

	QString const validName = utils::NameNormalizer::normalizeStrongly(name, false);
	if (validName.isEmpty()) {
		error(tr("Please enter valid c-style name for subprogram \"") + name + "\"");
		return;
	}

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
