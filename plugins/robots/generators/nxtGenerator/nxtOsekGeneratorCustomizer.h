#pragma once

#include <generatorBase/generatorCustomizer.h>
#include "nxtOsekGeneratorFactory.h"

namespace nxtOsek {

class NxtOsekGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	NxtOsekGeneratorCustomizer(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage);

	virtual generatorBase::GeneratorFactoryBase *factory();

private:
	NxtOsekGeneratorFactory mFactory;
};

}
