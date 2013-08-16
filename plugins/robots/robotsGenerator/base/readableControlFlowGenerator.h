#pragma once

#include "controlFlowGeneratorBase.h"

namespace qReal {
namespace robots {
namespace generators {

class ReadableControlFlowGenerator : public ControlFlowGeneratorBase
{
public:
	ReadableControlFlowGenerator(LogicalModelAssistInterface const &model
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer const &customizer
			, Id const &diagramId
			, QObject *parent = 0);
};

}
}
}
