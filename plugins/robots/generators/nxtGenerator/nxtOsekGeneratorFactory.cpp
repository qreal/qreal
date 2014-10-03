#include "nxtOsekGeneratorFactory.h"

#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>

#include "converters/nxtStringPropertyConverter.h"

using namespace nxtOsek;
using namespace generatorBase::simple;

NxtOsekGeneratorFactory::NxtOsekGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager)
{
}

NxtOsekGeneratorFactory::~NxtOsekGeneratorFactory()
{
}

generatorBase::simple::AbstractSimpleGenerator *NxtOsekGeneratorFactory::simpleGenerator(qReal::Id const &id
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

QString NxtOsekGeneratorFactory::pathToTemplates() const
{
	return ":/nxtOSEK/templates";
}

generatorBase::simple::Binding::ConverterInterface *NxtOsekGeneratorFactory::stringPropertyConverter() const
{
	return new converters::NxtStringPropertyConverter(*mVariables, *mSubprograms, *systemVariableNameConverter());
}
