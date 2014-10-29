#include "nxtGeneratorBase/nxtGeneratorFactory.h"

#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>

#include "simpleGenerators/clearScreenBlockGenerator.h"
#include "simpleGenerators/drawPixelBlockGenerator.h"
#include "simpleGenerators/drawRectBlockGenerator.h"
#include "simpleGenerators/drawLineBlockGenerator.h"
#include "simpleGenerators/drawCircleBlockGenerator.h"
#include "converters/nxtStringPropertyConverter.h"

using namespace nxt;
using namespace nxt::simple;
using namespace generatorBase::simple;

NxtGeneratorFactory::NxtGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, QString const &generatorName)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
	, mGeneratorName(generatorName)
	, mImages(pathToTemplates())
{
}

NxtGeneratorFactory::~NxtGeneratorFactory()
{
}

parts::Images &NxtGeneratorFactory::images()
{
	return mImages;
}

generatorBase::simple::AbstractSimpleGenerator *NxtGeneratorFactory::simpleGenerator(qReal::Id const &id
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
	} else if (elementType == "ClearScreen") {
		return new ClearScreenBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("DrawPixel")) {
		return new DrawPixelBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("DrawLine")) {
		return new DrawLineBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("DrawCircle")) {
		return new DrawCircleBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("DrawRect")) {
		return new DrawRectBlockGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QString NxtGeneratorFactory::pathToTemplates() const
{
	return ":/" + mGeneratorName + "/templates";
}

generatorBase::simple::Binding::ConverterInterface *NxtGeneratorFactory::stringPropertyConverter() const
{
	return new converters::NxtStringPropertyConverter(pathToTemplates()
			, *mVariables
			, *mSubprograms
			, *reservedVariableNameConverter());
}

