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
};

}
}
}
