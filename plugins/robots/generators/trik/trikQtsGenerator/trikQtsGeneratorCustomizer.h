#pragma once

#include <generatorBase/generatorCustomizer.h>

#include "trikQtsGeneratorFactory.h"

namespace trik {

class TrikQtsGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	TrikQtsGeneratorCustomizer(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	TrikQtsGeneratorFactory mFactory;
};

}
