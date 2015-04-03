#include "ev3GeneratorFactory.h"

#include "simpleGenerators/drawPixelGenerator.h"
#include "simpleGenerators/drawLineGenerator.h"
#include "simpleGenerators/drawRectGenerator.h"
#include "simpleGenerators/drawCircleGenerator.h"

using namespace ev3;
using namespace ev3::simple;
using namespace generatorBase::simple;

Ev3GeneratorFactory::Ev3GeneratorFactory(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
	, mGeneratorName(generatorName)
{
}

Ev3GeneratorFactory::~Ev3GeneratorFactory()
{
}

generatorBase::simple::AbstractSimpleGenerator *Ev3GeneratorFactory::simpleGenerator(const qReal::Id &id, generatorBase::GeneratorCustomizer &customizer)
{
	QString const elementType = id.element();
	if (elementType == "Ev3DrawLine") {
		return new DrawLineGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3DrawPixel") {
		return new DrawPixelGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3DrawRect") {
		return new DrawRectGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3DrawCircle") {
		return new DrawCircleGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QString Ev3GeneratorFactory::pathToTemplates() const
{
	return ":/" + mGeneratorName + "/templates";
}
