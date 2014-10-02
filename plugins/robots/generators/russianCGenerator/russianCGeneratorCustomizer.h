#pragma once

#include <generatorBase/generatorCustomizer.h>
#include "russianCGeneratorFactory.h"

namespace russianC {

class RussianCGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	RussianCGeneratorCustomizer(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	RussianCGeneratorFactory *mFactory;
};

}
