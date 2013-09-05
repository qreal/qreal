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

GeneratorFactoryBase::GeneratorFactoryBase(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter)
	: mRepo(repo)
	, mErrorReporter(errorReporter)
{
}

GeneratorFactoryBase::~GeneratorFactoryBase()
{
}

void GeneratorFactoryBase::initialize()
{
	initVariables();
	initSubprograms();
}

void GeneratorFactoryBase::initVariables()
{
	mVariables = new parts::Variables;
}

void GeneratorFactoryBase::initSubprograms()
{
	mSubprograms = new parts::Subprograms(mRepo, mErrorReporter
			, pathToTemplates(), nameNormalizerConverter());
}

parts::Variables *GeneratorFactoryBase::variablesInfo() const
{
	return mVariables;
}

parts::Variables *GeneratorFactoryBase::variables()
{
	return mVariables;
}

parts::Subprograms *GeneratorFactoryBase::subprograms()
{
	return mSubprograms;
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
