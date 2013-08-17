#pragma once

#include "controlFlowGeneratorBase.h"

namespace qReal {
namespace robots {
namespace generators {

class ReadableControlFlowGenerator : public ControlFlowGeneratorBase
{
public:
	ReadableControlFlowGenerator(
			LogicalModelAssistInterface const &logicalModel
			, GraphicalModelAssistInterface const &graphicalModel
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer const &customizer
			, Id const &diagramId
			, QObject *parent = 0);

	virtual void visitRegular(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links);
	virtual void visitConditional(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links);
	virtual void visitLoop(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links);
	virtual void visitSwitch(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links);
	virtual void visitFork(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links);
};

}
}
}
