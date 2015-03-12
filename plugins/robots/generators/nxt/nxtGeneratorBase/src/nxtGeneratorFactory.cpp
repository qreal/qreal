#include "nxtGeneratorBase/nxtGeneratorFactory.h"

#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>

#include "simpleGenerators/clearScreenBlockGenerator.h"
#include "simpleGenerators/drawPixelBlockGenerator.h"
#include "simpleGenerators/drawRectBlockGenerator.h"
#include "simpleGenerators/drawLineBlockGenerator.h"
#include "simpleGenerators/drawCircleBlockGenerator.h"
#include "simpleGenerators/waitForColorBlockGenerator.h"
#include "converters/colorConverter.h"

using namespace nxt;
using namespace nxt::simple;
using namespace generatorBase::simple;

NxtGeneratorFactory::NxtGeneratorFactory(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName)
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

generatorBase::simple::AbstractSimpleGenerator *NxtGeneratorFactory::simpleGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer)
{
	const QString elementType = id.element();
	if (elementType == "NxtWaitForButton") {
		return new WaitForButtonGenerator(mRepo, customizer, id, this);
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
	} else if (elementType.contains("WaitForColor")) {
		return new WaitForColorBlockGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QString NxtGeneratorFactory::pathToTemplates() const
{
	return ":/" + mGeneratorName + "/templates";
}

Binding::ConverterInterface *NxtGeneratorFactory::colorConverter() const
{
	return new converters::ColorConverter(pathToTemplates());
}
