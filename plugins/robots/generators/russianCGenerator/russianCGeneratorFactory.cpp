#include "russianCGeneratorFactory.h"

#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>

#include "converters/russianCStringPropertyConverter.h"

using namespace russianC;
using namespace generatorBase::simple;

RussianCGeneratorFactory::RussianCGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: generatorBase::GeneratorFactoryBase(repo, errorReporter, robotModelManager)
{
}

RussianCGeneratorFactory::~RussianCGeneratorFactory()
{
}

generatorBase::simple::AbstractSimpleGenerator *RussianCGeneratorFactory::simpleGenerator(qReal::Id const &id
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

QString RussianCGeneratorFactory::pathToTemplates() const
{
	return ":/russianC/templates";
}

generatorBase::simple::Binding::ConverterInterface *RussianCGeneratorFactory::stringPropertyConverter() const
{
	return new converters::RussianCStringPropertyConverter(*mVariables, *systemVariableNameConverter());
}
