#include "ev3GeneratorFactory.h"

#include "simpleGenerators/drawPixelGenerator.h"
#include "simpleGenerators/drawLineGenerator.h"
#include "simpleGenerators/drawRectGenerator.h"
#include "simpleGenerators/drawCircleGenerator.h"
#include "simpleGenerators/enginesGenerator.h"
#include "simpleGenerators/enginesStopGenerator.h"
#include "simpleGenerators/waitForButtonGenerator.h"
#include "simpleGenerators/waitForTouchSensorGenerator.h"

#include "converters/outputPortNameConverter.h"
#include "converters/goToBlockNumberConverter.h"

using namespace ev3;
using namespace ev3::simple;
using namespace ev3::converters;
using namespace generatorBase::simple;

Ev3GeneratorFactory::Ev3GeneratorFactory(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
	, mGeneratorName(generatorName)
	, mGoToBlockNumber(0)
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
	} else if (elementType.contains("EnginesForward") || elementType.contains("EnginesBackward")) {
		return new EnginesGenerator(mRepo, customizer, id, elementType, this);
	} else if (elementType == "Ev3EnginesStop") {
		return new EnginesStopGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3WaitForUp") {
		mGoToBlockNumber++;
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForUp.t", this);
	} else if (elementType == "Ev3WaitForEnter") {
		mGoToBlockNumber++;
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForEnter.t", this);
	} else if (elementType == "Ev3WaitForDown") {
		mGoToBlockNumber++;
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForDown.t", this);
	 } else if (elementType == "Ev3WaitForRight") {
		mGoToBlockNumber++;
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForRight.t", this);
	} else if (elementType == "Ev3WaitForLeft") {
		mGoToBlockNumber++;
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForLeft.t", this);
	} else if (elementType == "Ev3WaitForBack") {
		mGoToBlockNumber++;
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForBack.t", this);
	} else if (elementType == "Ev3WaitForTouchSensor") {
		mGoToBlockNumber++;
		return new WaitForTouchSensorGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

Binding::ConverterInterface *Ev3GeneratorFactory::outputPortNameConverter() const
{
	return new OutputPortNameConverter(pathToTemplates());
}

Binding::ConverterInterface *Ev3GeneratorFactory::goToBlockNumberConverter() const
{
	return new GoToBlockNumberConverter(QString::number(mGoToBlockNumber));
}

QString Ev3GeneratorFactory::pathToTemplates() const
{
	return ":/" + mGeneratorName + "/templates";
}
