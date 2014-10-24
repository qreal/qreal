#include "nxtOsekCGeneratorFactory.h"

#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>

#include "converters/nxtStringPropertyConverter.h"

using namespace nxtOsek;
using namespace generatorBase::simple;

NxtOsekCGeneratorFactory::NxtOsekCGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
{
}

NxtOsekCGeneratorFactory::~NxtOsekCGeneratorFactory()
{
}

generatorBase::simple::AbstractSimpleGenerator *NxtOsekCGeneratorFactory::simpleGenerator(qReal::Id const &id
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

QString NxtOsekCGeneratorFactory::pathToTemplates() const
{
	return ":/nxtOSEK/templates";
}

generatorBase::simple::Binding::ConverterInterface *NxtOsekCGeneratorFactory::stringPropertyConverter() const
{
	return new converters::NxtStringPropertyConverter(*mVariables, *mSubprograms, *reservedVariableNameConverter());
}
