#pragma once

#include <generatorBase/generatorCustomizer.h>

#include "ev3GeneratorFactory.h"

namespace ev3 {

class Ev3GeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	Ev3GeneratorCustomizer(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, QString const &generatorName);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	Ev3GeneratorFactory mFactory;
};

}
