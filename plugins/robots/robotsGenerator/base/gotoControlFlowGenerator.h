#pragma once

#include "controlFlowGeneratorBase.h"

namespace qReal {
namespace robots {
namespace generators {

class GotoControlFlowGenerator : public ControlFlowGeneratorBase
{
public:
	GotoControlFlowGenerator(
			LogicalModelAssistInterface const &logicalModel
			, GraphicalModelAssistInterface const &graphicalModel
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer const &customizer
			, Id const &diagramId
			, QObject *parent = 0);
};

}
}
}
