#include "generatorFactoryBase.h"
#include "generatorCustomizer.h"

#include "simpleGenerators/commentElementGenerator.h"
#include "simpleGenerators/ifElementGenerator.h"
#include "simpleGenerators/infiniteLoopGenerator.h"
#include "simpleGenerators/forLoopGenerator.h"
#include "simpleGenerators/whileLoopGenerator.h"
#include "simpleGenerators/bindingGenerator.h"
#include "simpleGenerators/functionElementGenerator.h"
#include "simpleGenerators/enginesGenerator.h"
#include "simpleGenerators/enginesStopGenerator.h"
#include "simpleGenerators/timerGenerator.h"
#include "simpleGenerators/beepGenerator.h"
#include "simpleGenerators/playToneGenerator.h"
#include "simpleGenerators/finalNodeGenerator.h"
#include "simpleGenerators/nullificationEncoderGenerator.h"
#include "simpleGenerators/waitForColorBlockGenerator.h"
#include "simpleGenerators/waitForColorIntensityBlockGenerator.h"
#include "simpleGenerators/drawPixelBlockGenerator.h"
#include "simpleGenerators/drawRectBlockGenerator.h"
#include "simpleGenerators/clearScreenBlockGenerator.h"
#include "simpleGenerators/drawLineBlockGenerator.h"
#include "simpleGenerators/drawCircleBlockGenerator.h"
#include "simpleGenerators/printTextBlockGenerator.h"
#include "simpleGenerators/waitForLightBlockGenerator.h"
#include "simpleGenerators/waitForButtonsBlockGenerator.h"
#include "simpleGenerators/waitForSonarBlockGenerator.h"
#include "simpleGenerators/waitForEncoderBlockGenerator.h"
#include "simpleGenerators/waitForTouchSensorBlockGenerator.h"
#include "simpleGenerators/waitForSoundBlockGenerator.h"
#include "simpleGenerators/waitForGyroscopeBlockGenerator.h"
#include "simpleGenerators/waitForAccelerometerBlockGenerator.h"
#include "simpleGenerators/initialNodeGenerator.h"
#include "simpleGenerators/subprogramsSimpleGenerator.h"
#include "simpleGenerators/breakGenerator.h"
#include "simpleGenerators/continueGenerator.h"

#include "converters/nameNormalizerConverter.h"
#include "converters/inequalitySignConverter.h"
#include "converters/colorConverter.h"
#include "converters/breakModeConverter.h"
#include "converters/inputPortConverter.h"
#include "converters/outputPortConverter.h"
#include "converters/enginePortsConverter.h"
#include "converters/typeConverter.h"
#include "converters/functionInvocationConverter.h"
#include "converters/functionBlockConverter.h"
#include "converters/intPropertyConverter.h"
#include "converters/floatPropertyConverter.h"
#include "converters/boolPropertyConverter.h"

using namespace qReal::robots::generators;
using namespace simple;

GeneratorFactoryBase::GeneratorFactoryBase(qrRepo::RepoApi const &repo)
	: mRepo(repo)
	, mVariables(new parts::Variables)
{
}

GeneratorFactoryBase::~GeneratorFactoryBase()
{
}

parts::Variables *GeneratorFactoryBase::variablesInfo() const
{
	return mVariables;
}

parts::Variables *GeneratorFactoryBase::variables()
{
	return mVariables;
}

simple::AbstractSimpleGenerator *GeneratorFactoryBase::ifGenerator(Id const &id
		, GeneratorCustomizer &customizer
		, bool elseIsEmpty
		, bool needInverting)
{
	return new IfElementGenerator(mRepo, customizer, id, elseIsEmpty, needInverting, this);
}

simple::AbstractSimpleGenerator *GeneratorFactoryBase::infiniteLoopGenerator(Id const &id
		, GeneratorCustomizer &customizer)
{
	return new InfiniteLoopGenerator(mRepo, customizer, id, this);
}

simple::AbstractSimpleGenerator *GeneratorFactoryBase::whileLoopGenerator(Id const &id
		, GeneratorCustomizer &customizer
		, bool doWhileForm
		, bool needInverting)
{
	return new WhileLoopGenerator(mRepo, customizer, id, doWhileForm, needInverting, this);
}

simple::AbstractSimpleGenerator *GeneratorFactoryBase::forLoopGenerator(Id const &id
		, GeneratorCustomizer &customizer)
{
	return new ForLoopGenerator(mRepo, customizer, id, this);
}

AbstractSimpleGenerator *GeneratorFactoryBase::simpleGenerator(qReal::Id const &id
		, GeneratorCustomizer &customizer)
{
	QString const elementType = id.element();

	if (elementType == "CommentBlock") {
		return new CommentElementGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Function") {
		return new FunctionElementGenerator(mRepo, customizer, id, this);
	} else if (elementType == "EnginesForward" || elementType == "EnginesBackward") {
		return new EnginesGenerator(mRepo, customizer, id, elementType, this);
	} else if (elementType == "EnginesStop") {
		return new EnginesStopGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Timer") {
		return new TimerGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Beep") {
		return new BeepGenerator(mRepo, customizer, id, this);
	} else if (elementType == "PlayTone") {
		return new PlayToneGenerator(mRepo, customizer, id, this);
	} else if (elementType == "InitialNode") {
		return new InitialNodeGenerator(mRepo, customizer, id, this);
	} else if (elementType == "FinalNode") {
		return new FinalNodeGenerator(mRepo, customizer, id, this);
	} else if (elementType == "NullificationEncoder") {
		return new NullificationEncoderGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForColor") {
		return new WaitForColorBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForColorIntensity") {
		return new WaitForColorIntensityBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForLight") {
		return new WaitForLightBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForTouchSensor") {
		return new WaitForTouchSensorBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForSonarDistance") {
		return new WaitForSonarBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForEncoder") {
		return new WaitForEncoderBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForSound") {
		return new WaitForSoundBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForGyroscope") {
		return new WaitForGyroscopeBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForAccelerometer") {
		return new WaitForAccelerometerBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForButtons") {
		return new WaitForButtonsBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "DrawPixel") {
		return new DrawPixelBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "DrawLine") {
		return new DrawLineBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "DrawCircle") {
		return new DrawCircleBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "PrintText") {
		return new PrintTextBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "DrawRect") {
		return new DrawRectBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "ClearScreen") {
		return new ClearScreenBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Subprogram") {
		return new SubprogramsSimpleGenerator(mRepo, customizer, id, this);
	}

	return NULL;
}

simple::AbstractSimpleGenerator *GeneratorFactoryBase::breakGenerator(Id const &id
		, GeneratorCustomizer &customizer)
{
	return new BreakGenerator(mRepo, customizer, id, this);
}

simple::AbstractSimpleGenerator *GeneratorFactoryBase::continueGenerator(Id const &id
		, GeneratorCustomizer &customizer)
{
	return new ContinueGenerator(mRepo, customizer, id, this);
}

// Converters can be instantiated without taking ownership because bindinders do this

Binding::ConverterInterface *GeneratorFactoryBase::intPropertyConverter() const
{
	return new converters::IntPropertyConverter(pathToTemplates()
			, inputPortConverter()
			, outputPortConverter()
			, functionInvocationConverter()
			, typeConverter()
			, variablesInfo());
}

Binding::ConverterInterface *GeneratorFactoryBase::floatPropertyConverter() const
{
	return new converters::FloatPropertyConverter(pathToTemplates()
			, intPropertyConverter()
			, outputPortConverter()
			, functionInvocationConverter());
}

Binding::ConverterInterface *GeneratorFactoryBase::boolPropertyConverter(bool needInverting) const
{
	return new converters::BoolPropertyConverter(pathToTemplates()
			, intPropertyConverter()
			, outputPortConverter()
			, functionInvocationConverter()
			, needInverting);
}

Binding::ConverterInterface *GeneratorFactoryBase::stringPropertyConverter() const
{
	return new Binding::EmptyConverter;
}

Binding::ConverterInterface *GeneratorFactoryBase::nameNormalizerConverter() const
{
	return new converters::NameNormalizerConverter;
}

Binding::ConverterInterface *GeneratorFactoryBase::functionInvocationConverter() const
{
	return new converters::FunctionInvocationConverter(pathToTemplates());
}

Binding::ConverterInterface *GeneratorFactoryBase::functionBlockConverter() const
{
	return new converters::FunctionBlockConverter(pathToTemplates()
			, inputPortConverter()
			, outputPortConverter()
			, functionInvocationConverter());
}

Binding::ConverterInterface *GeneratorFactoryBase::inequalitySignConverter() const
{
	return new converters::InequalitySignConverter(pathToTemplates());
}

Binding::MultiConverterInterface *GeneratorFactoryBase::enginesConverter() const
{
	return new converters::EnginePortsConverter(pathToTemplates(), outputPortConverter());
}

Binding::ConverterInterface *GeneratorFactoryBase::inputPortConverter() const
{
	return new converters::InputPortConverter(pathToTemplates());
}

Binding::ConverterInterface *GeneratorFactoryBase::outputPortConverter() const
{
	return new converters::OutputPortConverter(pathToTemplates());
}

Binding::ConverterInterface *GeneratorFactoryBase::colorConverter() const
{
	return new converters::ColorConverter(pathToTemplates());
}

Binding::ConverterInterface *GeneratorFactoryBase::breakModeConverter() const
{
	return new converters::BreakModeConverter(pathToTemplates());
}

Binding::ConverterInterface *GeneratorFactoryBase::typeConverter() const
{
	return new converters::TypeConverter(pathToTemplates());
}

/*
AbstractSimpleGenerator *GeneratorFactoryBase::templateGenratorFor(Id const &id
		, QString const &templateFile, QList<Binding *> const &bindings)
{
	return new BindingGenerator(mModel, id, pathToTemplates() + '/' + templateFile, bindings, this);
}

AbstractSimpleGenerator *GeneratorFactoryBase::ifBlock(Id const &id) const
{
	return templateGenratorFor(id, "ifBlock.t", QList<Binding *>()
			<< new Binding("@@CONDITION@@", "Condition", boolPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::loop(Id const &id) const
{
	return templateGenratorFor(id, "loop.t", QList<Binding *>()
			<< new Binding("@@CONDITION@@", "Condition", boolPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::function(Id const &id) const
{
	return templateGenratorFor(id, "function.t", QList<Binding *>()
			<< new Binding("@@BODY@@", "Body", functionBlockConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::comment(Id const &id) const
{
	return NULL;
}

AbstractSimpleGenerator *GeneratorFactoryBase::enginesForward(Id const &id) const
{
	return templateGenratorFor(id, "enginesForward.t", QList<Binding *>()
			<< new Binding("@@POWER@@", "Power", intPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::enginesBackward(Id const &id) const
{
	return templateGenratorFor(id, "enginesBackward.t", QList<Binding *>()
			<< new Binding("@@POWER@@", "Power", intPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::enginesStop(Id const &id) const
{
	return templateGenratorFor(id, "enginesStop.t", QList<Binding *>()
			<< new Binding("@@POWER@@", "Power", intPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::timer(Id const &id) const
{
	return templateGenratorFor(id, "timer.t", QList<Binding *>()
			<< new Binding("@@DELAY@@", "Delay", intPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::beep(Id const &id) const
{
	return templateGenratorFor(id, "beep.t", QList<Binding *>()
			<< new Binding("@@VOLUME@@", "Volume", intPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::playTone(Id const &id) const
{
	return templateGenratorFor(id, "playTone.t", QList<Binding *>()
			<< new Binding("@@FREQUENCY@@", "Frequency", intPropertyConverter())
			<< new Binding("@@VOLUME@@", "Volume", intPropertyConverter())
			<< new Binding("@@DURATION@@", "Duration", intPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::finalNode(Id const &id) const
{
	templateGenratorFor(id, "finalBlock.t", QList<Binding *>());
}

AbstractSimpleGenerator *GeneratorFactoryBase::nullificationEncoder(Id const &id) const
{
	templateGenratorFor(id, "nullificationEncoder.t", QList<Binding *>());
}

AbstractSimpleGenerator *GeneratorFactoryBase::waitForColor(Id const &id) const
{
	templateGenratorFor(id, "waitForColor.t", QList<Binding *>());
}

AbstractSimpleGenerator *GeneratorFactoryBase::waitForColorIntensity(Id const &id) const
{
	return templateGenratorFor(id, "waitForColorIntensity.t", QList<Binding *>()
			<< new Binding("@@INTENSITY@@", "Intensity", intPropertyConverter())
			<< new Binding("@@SIGN@@", "Sign", inequalitySignConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::waitForLight(Id const &id) const
{
	return templateGenratorFor(id, "waitForLight.t", QList<Binding *>()
			<< new Binding("@@PERCENTS@@", "Percents", intPropertyConverter())
			<< new Binding("@@SIGN@@", "Sign", inequalitySignConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::waitForTouchSensor(Id const &id) const
{
	return templateGenratorFor(id, "waitForTouchSensor.t", QList<Binding *>());
}

AbstractSimpleGenerator *GeneratorFactoryBase::waitForSonarDistance(Id const &id) const
{
	return templateGenratorFor(id, "waitForSonarDistance.t", QList<Binding *>()
			<< new Binding("@@DISTANCE@@", "Distance", intPropertyConverter())
			<< new Binding("@@SIGN@@", "Sign", inequalitySignConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::waitForEncoder(Id const &id) const
{
	return templateGenratorFor(id, "waitForEncoder.t", QList<Binding *>()
			<< new Binding("@@TACHO_LIMIT@@", "TachoLimit", intPropertyConverter())
			<< new Binding("@@SIGN@@", "Sign", inequalitySignConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::waitForSound(Id const &id) const
{
	return templateGenratorFor(id, "waitForSound.t", QList<Binding *>()
			<< new Binding("@@VOLUME@@", "Volume", intPropertyConverter())
			<< new Binding("@@SIGN@@", "Sign", inequalitySignConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::waitForGyroscope(Id const &id) const
{
	return templateGenratorFor(id, "waitForGyroscope.t", QList<Binding *>()
			<< new Binding("@@DEGREES@@", "Degrees", intPropertyConverter())
			<< new Binding("@@SIGN@@", "Sign", inequalitySignConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::waitForAccelerometer(Id const &id) const
{
	// TODO:
	return NULL;
}

AbstractSimpleGenerator *GeneratorFactoryBase::balance(Id const &id) const
{
	// TODO: Maybe remove balancers at all?
	return NULL;
}

AbstractSimpleGenerator *GeneratorFactoryBase::balanceInit(Id const &id) const
{
	// TODO: Maybe remove balancers at all?
	return NULL;
}

AbstractSimpleGenerator *GeneratorFactoryBase::waitForButtons(Id const &id) const
{
	return templateGenratorFor(id, "waitForButtons.t", QList<Binding *>()
			<< new Binding("@@RIGHT_BUTTON_CLICKS@@", "RightButtonClicks", intPropertyConverter())
			<< new Binding("@@LEFT_BUTTON_CLICKS@@", "LeftButtonClicks", intPropertyConverter())
			<< new Binding("@@CENTRAL_BUTTON_CLICKS@@", "CentralButtonClicks", intPropertyConverter())
			<< new Binding("@@BOTTOM_BUTTON_CLICKS@@", "BottomButtonClicks", intPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::drawPixel(Id const &id) const
{
	return templateGenratorFor(id, "drawPixel.t", QList<Binding *>()
			<< new Binding("@@X@@", "XCoordinatePix", intPropertyConverter())
			<< new Binding("@@Y@@", "YCoordinatePix", intPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::drawLine(Id const &id) const
{
	return templateGenratorFor(id, "drawLine.t", QList<Binding *>()
			<< new Binding("@@X1@@", "X1CoordinateLine", intPropertyConverter())
			<< new Binding("@@Y1@@", "Y1CoordinateLine", intPropertyConverter())
			<< new Binding("@@X2@@", "X2CoordinateLine", intPropertyConverter())
			<< new Binding("@@Y2@@", "Y2CoordinateLine", intPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::drawCircle(Id const &id) const
{
	return templateGenratorFor(id, "drawCircle.t", QList<Binding *>()
			<< new Binding("@@X@@", "XCoordinateCircle", intPropertyConverter())
			<< new Binding("@@Y@@", "YCoordinateCircle", intPropertyConverter())
			<< new Binding("@@RADIUS@@", "CircleRadius", intPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::printText(Id const &id) const
{
	return templateGenratorFor(id, "printText.t", QList<Binding *>()
			<< new Binding("@@X@@", "XCoordinateText", intPropertyConverter())
			<< new Binding("@@Y@@", "YCoordinateText", intPropertyConverter())
			<< new Binding("@@TEXT@@", "PrintText", stringPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::drawRect(Id const &id) const
{
	return templateGenratorFor(id, "drawRect.t", QList<Binding *>()
			<< new Binding("@@X@@", "XCoordinateRect", intPropertyConverter())
			<< new Binding("@@Y@@", "YCoordinateRect", intPropertyConverter())
			<< new Binding("@@WIDTH@@", "WidthRect", intPropertyConverter())
			<< new Binding("@@HEIGHT@@", "HeightRect", intPropertyConverter()));
}

AbstractSimpleGenerator *GeneratorFactoryBase::clearScreen(Id const &id) const
{
	return templateGenratorFor(id, "clearScreen.t", QList<Binding *>());
}

AbstractSimpleGenerator *GeneratorFactoryBase::subprogram(Id const &id) const
{
	return templateGenratorFor(id, "subprogramCall.t", QList<Binding *>()
			<< new Binding("@@NAME@@", "name", nameNormalizerConverter()));
}
*/
