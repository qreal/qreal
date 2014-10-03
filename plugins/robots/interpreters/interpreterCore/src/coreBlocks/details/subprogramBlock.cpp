#include "subprogramBlock.h"

#include <utils/tracer.h>
#include <qrutils/nameNormalizer.h>

using namespace qReal;
using namespace interpreterCore::coreBlocks::details;

SubprogramBlock::SubprogramBlock()
{
}

void SubprogramBlock::run()
{
	Id const logicalId = mGraphicalModelApi->logicalId(id());

	QString const name = mLogicalModelApi->name(logicalId);
	utils::Tracer::debug(utils::Tracer::blocks, "SubprogramBlock::run", "stepping into " + name);

	QString const validName = utils::NameNormalizer::normalizeStrongly(name, false);
	if (validName.isEmpty()) {
		error(tr("Please enter valid c-style name for subprogram \"") + name + "\"");
		return;
	}

	Id const logicalDiagram = mLogicalModelApi->logicalRepoApi().outgoingExplosion(logicalId);
	IdList const diagrams = mGraphicalModelApi->graphicalIdsByLogicalId(logicalDiagram);
	if (!diagrams.isEmpty()) {
		emit stepInto(diagrams[0]);
	}
}

void SubprogramBlock::finishedSteppingInto()
{
	utils::Tracer::debug(utils::Tracer::blocks, "SubprogramBlock::run"
			, "finished stepping into " + stringProperty(id(), "name"));
	emit done(mNextBlockId);
}
