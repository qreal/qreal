#pragma once

#include <generatorBase/generatorCustomizer.h>

#include "nxtOsekCGeneratorFactory.h"

namespace nxtOsek {

class NxtOsekCGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	NxtOsekCGeneratorCustomizer(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor);

	virtual generatorBase::GeneratorFactoryBase *factory();

private:
	NxtOsekCGeneratorFactory mFactory;
};

}
