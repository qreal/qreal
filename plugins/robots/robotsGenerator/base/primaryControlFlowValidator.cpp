#include "primaryControlFlowValidator.h"

using namespace qReal::robots::generators;

PrimaryControlFlowValidator::PrimaryControlFlowValidator(
		LogicalModelAssistInterface const &logicalModel
		, GraphicalModelAssistInterface const &graphicalModel
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer const &customizer
		, Id const &diagramId)
	: mLogicalModel(logicalModel)
	, mGraphicalModel(graphicalModel)
	, mErrorReporter(errorReporter)
	, mCustomizer(customizer)
	, mDiagram(diagramId)
	, mDfser(logicalModel)
{
}

PrimaryControlFlowValidator::~PrimaryControlFlowValidator()
{
}

bool PrimaryControlFlowValidator::validate()
{
	mIfBranches.clear();

	findInitialNode();
	if (mFirstId.isNull()) {
		error(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node"), mDiagram);
		return false;
	}

	mErrorsOccured = false;
	mDfser.startSearch(mFirstId, this);

	return !mErrorsOccured;
}

qReal::Id PrimaryControlFlowValidator::initialId() const
{
	return mFirstId;
}

QPair<qReal::Id, qReal::Id> PrimaryControlFlowValidator::ifBranchesFor(qReal::Id const &id) const
{
	return mIfBranches[id];
}

void PrimaryControlFlowValidator::visit(qReal::Id const &nodeId
		, QList<DeepFirstSearcher::LinkInfo> const &links)
{
	switch (mCustomizer.semanticsOf(nodeId)) {
	case enums::semantics::regularBlock:
		validateRegular(nodeId, links);
		break;
	case enums::semantics::condidionalBlock:
		validateConditional(nodeId, links);
		break;
	case enums::semantics::loopBlock:
		validateLoop(nodeId, links);
		break;
	case enums::semantics::switchBlock:
		validateSwitch(nodeId, links);
		break;
	case enums::semantics::forkBlock:
		validateFork(nodeId, links);
		break;
	default:
		error(QObject::tr("Unknown block type"), nodeId);
		break;
	}
}

void PrimaryControlFlowValidator::validateRegular(Id const &id
		, QList<DeepFirstSearcher::LinkInfo> const &links)
{
	if (mCustomizer.isFinalNode(id)) {
		validateFinalBlock(id, links);
		return;
	}

	if (links.size() != 1) {
		error(QObject::tr("This element must have exactly ONE outgoing link"), id);
	} else {
		checkForConnected(links[0]);
	}
}

void PrimaryControlFlowValidator::validateFinalBlock(Id const &id
		, QList<DeepFirstSearcher::LinkInfo> const &links)
{
	if (!links.isEmpty()) {
		error(QObject::tr("Final node must not have outgioing links"), id);
	}
}

void PrimaryControlFlowValidator::validateConditional(Id const &id
		, QList<DeepFirstSearcher::LinkInfo> const &links)
{
	if (links.size() != 2) {
		error(QObject::tr("If block must have exactly TWO outgoing links"), id);
		return;
	}

	// In correct case exactly 2 of this 3 would be non-null
	Id trueBlock, falseBlock, nonMarkedBlock;

	foreach (DeepFirstSearcher::LinkInfo const &link, links) {
		checkForConnected(link);

		switch(link.guard) {
		case DeepFirstSearcher::trueGuard:
			if (trueBlock.isNull()) {
				trueBlock = link.target;
			} else {
				error(QObject::tr("Two outgoing links marked with 'true' found"), id);
			}
			break;

		case DeepFirstSearcher::falseGuard:
			if (falseBlock.isNull()) {
				falseBlock = link.target;
			} else {
				error(QObject::tr("Two outgoing links marked with 'false' found"), id);
			}
			break;

		default:
			if (nonMarkedBlock.isNull()) {
				nonMarkedBlock = link.target;
			} else {
				error(QObject::tr("There must be a link with property \"Guard\""\
						" set to one of the conditions"), id);
			}
			break;
		}
	}

	// Now we have correctly linked branches. Determining who is who...
	QPair<Id, Id> branches;
	if (trueBlock.isNull()) {
		branches.first = nonMarkedBlock;
		branches.second = falseBlock;
	} else if (falseBlock.isNull()) {
		branches.first = trueBlock;
		branches.second = nonMarkedBlock;
	} else {
		branches.first = trueBlock;
		branches.second = falseBlock;
	}

	mIfBranches[id] = branches;
}

void PrimaryControlFlowValidator::validateLoop(Id const &id
		, QList<DeepFirstSearcher::LinkInfo> const &links)
{
	if (links.size() != 2) {
		error(QObject::tr("Loop block must have exactly TWO outgoing links"), id);
		return;
	}

	// In correct case must be non-null and different
	Id iterationBlock, nonMarkedBlock;

	foreach (DeepFirstSearcher::LinkInfo const &link, links) {
		checkForConnected(link);

		switch (link.guard) {
		case DeepFirstSearcher::iterationGuard:
			if (iterationBlock.isNull()) {
				iterationBlock = link.target;
			} else {
				error(QObject::tr("Two outgoing links marked with \"iteration\" found"), id);
				return;
			}
			break;
		default:
			if (nonMarkedBlock.isNull()) {
				nonMarkedBlock = link.target;
			} else {
				error(QObject::tr("There must be a link with property \"Guard\""\
						" set to \"iteration\""), id);
				return;
			}
			break;
		}
	}

	if (iterationBlock == nonMarkedBlock) {
		error(QObject::tr("Outgoing links from loop block must be connected to different blocks"), id);
	}
}

void PrimaryControlFlowValidator::validateSwitch(Id const &id
		, QList<DeepFirstSearcher::LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
	// TODO
}

void PrimaryControlFlowValidator::validateFork(Id const &id
		, QList<DeepFirstSearcher::LinkInfo> const &links)
{
	if (links.size() < 2) {
		error(QObject::tr("Fork block must have at least TWO outgoing links"), id);
		return;
	}

	foreach (DeepFirstSearcher::LinkInfo const &link, links) {
		checkForConnected(link);
	}
}

void PrimaryControlFlowValidator::error(QString const &message, qReal::Id const &id)
{
	mErrorReporter.addError(message, id);
	// Returns false for possibility of one-line 'return error(...);'
	mErrorsOccured = true;
}

bool PrimaryControlFlowValidator::checkForConnected(DeepFirstSearcher::LinkInfo const &link)
{
	if (!link.connected) {
		error(QObject::tr("Outgoing link is not connected"), link.linkId);
		return false;
	}

	return true;
}

void PrimaryControlFlowValidator::findInitialNode()
{
	qReal::IdList const initialNodes(mGraphicalModel.children(mDiagram));
	foreach (qReal::Id const &initialNode, initialNodes) {
		if (mCustomizer.isInitialNode(initialNode)) {
			mFirstId = mGraphicalModel.logicalId(initialNode);
			return;
		}
	}

	mFirstId = Id();
}
