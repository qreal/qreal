#pragma once

#include <generatorBase/generatorCustomizer.h>

#include "nxtGeneratorBase/nxtGeneratorFactory.h"

namespace nxt {

class NxtGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	NxtGeneratorCustomizer(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, const QString &generatorName);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	NxtGeneratorFactory mFactory;
};

}
