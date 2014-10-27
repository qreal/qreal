#pragma once

#include <generatorBase/generatorCustomizer.h>

#include "nxtGeneratorFactory.h"

namespace nxt {

class NxtGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	NxtGeneratorCustomizer(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, QString const &generatorName);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	NxtGeneratorFactory mFactory;
};

}
