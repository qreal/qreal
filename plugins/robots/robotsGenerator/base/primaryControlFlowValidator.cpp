#include "primaryControlFlowValidator.h"

using namespace qReal::robots::generators;

PrimaryControlFlowValidator::PrimaryControlFlowValidator(
		LogicalModelAssistInterface const &model
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer const &customizer
		, Id const &diagramId)
	: mModel(model)
	, mErrorReporter(errorReporter)
	, mCustomizer(customizer)
	, mDiagram(diagramId)
	, mDfser(model)
{
}

PrimaryControlFlowValidator::~PrimaryControlFlowValidator()
{
}

bool PrimaryControlFlowValidator::validate()
{
	Id const initialNode = findInitialNode();
	if (initialNode.isNull()) {
		error(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node"), mDiagram);
		return false;
	}

	mErrorsOccured = false;
	mDfser.startSearch(initialNode, this);

	return mErrorsOccured;
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
		error(QObject::tr("This element is allowed to have exactly one outgoing link"), id);
	}

	checkForConnected(links[0]);
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

	// In correct case exactly 2 of this 3 would be true
	bool trueFound = false, falseFound = false, nonMarkedFound = false;

	foreach (DeepFirstSearcher::LinkInfo const &link, links) {
		checkForConnected(link);

		switch (link.guard) {
		case DeepFirstSearcher::trueGuard:
			if (trueFound) {
				error(QObject::tr("Two outgoing links marked with 'true' found"), id);
			} else {
				trueFound = true;
			}

			break;
		case DeepFirstSearcher::falseGuard:
			if (falseFound) {
				error(QObject::tr("Two outgoing links marked with 'false' found"), id);
			} else {
				falseFound = true;
			}

			break;
		default:
			if (nonMarkedFound) {
				error(QObject::tr("There must be a link with property \"Guard\""\
						"set to one of the conditions"), id);
			} else {
				nonMarkedFound = true;
			}

			break;
		}
	}
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
						"set to \"iteration\""), id);
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

qReal::Id PrimaryControlFlowValidator::findInitialNode() const
{
	qReal::IdList const initialNodes(mModel.children(mDiagram));
	foreach (qReal::Id const &initialNode, initialNodes) {
		if (mCustomizer.isInitialNode(initialNode)) {
			return initialNode;
		}
	}

	return Id();
}
