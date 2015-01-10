#include "subprogramBlock.h"

#include <qrutils/nameNormalizer.h>

using namespace qReal::interpretation::blocks;

SubprogramBlock::SubprogramBlock()
{
}

void SubprogramBlock::run()
{
	Id const logicalId = mGraphicalModelApi->logicalId(id());

	QString const name = mLogicalModelApi->name(logicalId);

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
	emit done(mNextBlockId);
}
