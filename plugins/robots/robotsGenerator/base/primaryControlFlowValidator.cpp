#include "primaryControlFlowValidator.h"

using namespace qReal::robots::generators;

PrimaryControlFlowValidator::PrimaryControlFlowValidator(
		LogicalModelAssistInterface const &logicalModel
		, GraphicalModelAssistInterface const &graphicalModel
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer const &customizer
		, Id const &diagramId)
	: RobotsDiagramVisitor(logicalModel, customizer)
	, mLogicalModel(logicalModel)
	, mGraphicalModel(graphicalModel)
	, mErrorReporter(errorReporter)
	, mCustomizer(customizer)
	, mDiagram(diagramId)
{
}

PrimaryControlFlowValidator::~PrimaryControlFlowValidator()
{
}

bool PrimaryControlFlowValidator::validate()
{
	mIfBranches.clear();
	mLoopBranches.clear();

	findInitialNode();
	if (mInitialNode.isNull()) {
		error(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node"), mDiagram);
		return false;
	}

	mErrorsOccured = false;
	startSearch(mInitialNode);

	return !mErrorsOccured;
}

qReal::Id PrimaryControlFlowValidator::initialNode() const
{
	return mInitialNode;
}

QPair<qReal::Id, qReal::Id> PrimaryControlFlowValidator::ifBranchesFor(qReal::Id const &id) const
{
	return mIfBranches[id];
}

QPair<qReal::Id, qReal::Id> PrimaryControlFlowValidator::loopBranchesFor(qReal::Id const &id) const
{
	return mLoopBranches[id];
}

void PrimaryControlFlowValidator::visitRegular(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	if (links.size() != 1) {
		error(QObject::tr("This element must have exactly ONE outgoing link"), id);
	} else {
		checkForConnected(links[0]);
	}
}

void PrimaryControlFlowValidator::visitFinal(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	if (!links.isEmpty()) {
		error(QObject::tr("Final node must not have outgioing links"), id);
	}
}

void PrimaryControlFlowValidator::visitConditional(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	if (links.size() != 2) {
		error(QObject::tr("If block must have exactly TWO outgoing links"), id);
		return;
	}

	// In correct case exactly 2 of this 3 would be non-null
	Id trueBlock, falseBlock, nonMarkedBlock;

	foreach (utils::DeepFirstSearcher::LinkInfo const &link, links) {
		checkForConnected(link);

		switch (guardOf(link.linkId)) {
		case trueGuard:
			if (trueBlock.isNull()) {
				trueBlock = link.target;
			} else {
				error(QObject::tr("Two outgoing links marked with 'true' found"), id);
			}
			break;

		case falseGuard:
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

void PrimaryControlFlowValidator::visitLoop(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	if (links.size() != 2) {
		error(QObject::tr("Loop block must have exactly TWO outgoing links"), id);
		return;
	}

	// In correct case must be non-null and different
	Id iterationBlock, nonMarkedBlock;

	foreach (utils::DeepFirstSearcher::LinkInfo const &link, links) {
		checkForConnected(link);

		switch (guardOf(link.linkId)) {
		case iterationGuard:
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

	mLoopBranches[id] = qMakePair(iterationBlock, nonMarkedBlock);
}

void PrimaryControlFlowValidator::visitSwitch(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
	// TODO
}

void PrimaryControlFlowValidator::visitFork(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	if (links.size() < 2) {
		error(QObject::tr("Fork block must have at least TWO outgoing links"), id);
		return;
	}

	foreach (utils::DeepFirstSearcher::LinkInfo const &link, links) {
		checkForConnected(link);
	}
}

void PrimaryControlFlowValidator::visitUnknown(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	Q_UNUSED(links)
	error(QObject::tr("Unknown block type"), id);
}

void PrimaryControlFlowValidator::error(QString const &message, qReal::Id const &id)
{
	mErrorReporter.addError(message, id);
	// Returns false for possibility of one-line 'return error(...);'
	mErrorsOccured = true;
}

bool PrimaryControlFlowValidator::checkForConnected(utils::DeepFirstSearcher::LinkInfo const &link)
{
	if (!link.connected) {
		error(QObject::tr("Outgoing link is not connected"), link.linkId);
		return false;
	}

	return true;
}

void PrimaryControlFlowValidator::findInitialNode()
{
	qReal::IdList const diagramNodes(mLogicalModel.children(mDiagram));
	foreach (qReal::Id const &diagramNode, diagramNodes) {
		if (mCustomizer.isInitialNode(diagramNode)) {
			mInitialNode = mGraphicalModel.logicalId(diagramNode);
			return;
		}
	}

	mInitialNode = Id();
}
