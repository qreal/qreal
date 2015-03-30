#pragma once

#include <generatorBase/generatorCustomizer.h>

#include "trikGeneratorFactory.h"

namespace trik {

class TrikGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	TrikGeneratorCustomizer(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, const QString &generatorName);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	TrikGeneratorFactory mFactory;
};

}
