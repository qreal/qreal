#pragma once

#include "controlFlowGeneratorBase.h"

namespace qReal {
namespace robots {
namespace generators {

class GotoControlFlowGenerator : public ControlFlowGeneratorBase
{
public:
	GotoControlFlowGenerator(
			qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, Id const &diagramId
			, QObject *parent = 0);
};

}
}
}
