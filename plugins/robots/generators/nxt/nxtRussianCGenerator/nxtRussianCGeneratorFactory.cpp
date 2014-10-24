#include "nxtRussianCGeneratorFactory.h"

#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>

#include "converters/russianCStringPropertyConverter.h"

using namespace russianC;
using namespace generatorBase::simple;

NxtRussianCGeneratorFactory::NxtRussianCGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor)
	: generatorBase::GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
{
}

NxtRussianCGeneratorFactory::~NxtRussianCGeneratorFactory()
{
}

generatorBase::simple::AbstractSimpleGenerator *NxtRussianCGeneratorFactory::simpleGenerator(qReal::Id const &id
		, generatorBase::GeneratorCustomizer &customizer)
{
	QString const elementType = id.element();
	if (elementType == "NxtWaitForEnter") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForEnter.t", this);
	} else if (elementType == "NxtWaitForEscape") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForEscape.t", this);
	} else if (elementType == "NxtWaitForLeft") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForLeft.t", this);
	} else if (elementType == "NxtWaitForRight") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForRight.t", this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QString NxtRussianCGeneratorFactory::pathToTemplates() const
{
	return ":/russianC/templates";
}

generatorBase::simple::Binding::ConverterInterface *NxtRussianCGeneratorFactory::stringPropertyConverter() const
{
	return new converters::RussianCStringPropertyConverter(*mVariables, *reservedVariableNameConverter());
}
