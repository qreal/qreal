#pragma once

#include <generatorBase/generatorCustomizer.h>
#include "nxtRussianCGeneratorFactory.h"

namespace russianC {

class NxtRussianCGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	NxtRussianCGeneratorCustomizer(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	NxtRussianCGeneratorFactory *mFactory;
};

}
