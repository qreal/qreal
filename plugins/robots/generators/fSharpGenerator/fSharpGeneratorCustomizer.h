#pragma once

#include <generatorBase/generatorCustomizer.h>
#include "fSharpGeneratorFactory.h"

namespace fSharp {

class FSharpGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	FSharpGeneratorCustomizer(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	FSharpGeneratorFactory mFactory;
};

}
