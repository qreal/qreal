#include "generatorBase/generatorFactoryBase.h"
#include "generatorBase/generatorCustomizer.h"

#include <interpreterBase/robotModel/robotModelUtils.h>

#include "simpleGenerators/nullGenerator.h"
#include "simpleGenerators/commentElementGenerator.h"
#include "simpleGenerators/ifElementGenerator.h"
#include "simpleGenerators/infiniteLoopGenerator.h"
#include "simpleGenerators/forLoopGenerator.h"
#include "simpleGenerators/whileLoopGenerator.h"
#include "simpleGenerators/forkCallGenerator.h"
#include "simpleGenerators/switchGenerator.h"
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
#include "simpleGenerators/labelGenerator.h"
#include "simpleGenerators/gotoSimpleGenerator.h"
#include "simpleGenerators/variableInitGenerator.h"

#include "converters/nameNormalizerConverter.h"
#include "converters/inequalitySignConverter.h"
#include "converters/colorConverter.h"
#include "converters/breakModeConverter.h"
#include "converters/inputPortConverter.h"
#include "converters/outputPortConverter.h"
#include "converters/enginePortsConverter.h"
#include "converters/typeConverter.h"
#include "converters/reservedVariablesConverter.h"
#include "converters/functionBlockConverter.h"
#include "converters/intPropertyConverter.h"
#include "converters/floatPropertyConverter.h"
#include "converters/boolPropertyConverter.h"
#include "converters/switchConditionsMerger.h"
#include "generatorBase/converters/stringPropertyConverter.h"

#include "generatorBase/parts/variables.h"
#include "generatorBase/parts/subprograms.h"
#include "generatorBase/parts/threads.h"
#include "generatorBase/parts/engines.h"
#include "generatorBase/parts/sensors.h"
#include "generatorBase/parts/functions.h"
#include "generatorBase/parts/images.h"

#include "generatorBase/lua/luaProcessor.h"

using namespace generatorBase;
using namespace qReal;
using namespace simple;
using namespace interpreterBase::robotModel;

GeneratorFactoryBase::GeneratorFactoryBase(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, RobotModelManagerInterface const &robotModelManager
		, lua::LuaProcessor &luaProcessor)
	: mRepo(repo)
	, mErrorReporter(errorReporter)
	, mRobotModelManager(robotModelManager)
	, mLuaTranslator(luaProcessor)
{
}

GeneratorFactoryBase::~GeneratorFactoryBase()
{
	delete mDeviceVariables;
}

void GeneratorFactoryBase::initialize()
{
	mLuaTranslator.setPathToTemplates(pathToTemplates());
	initVariables();
	initSubprograms();
	mThreads = new parts::Threads(pathToTemplates());
	initEngines();
	initSensors();
	initFunctions();
	initImages();
	initDeviceVariables();
}

void GeneratorFactoryBase::setMainDiagramId(Id const &diagramId)
{
	mDiagram = diagramId;
	mSensors->reinit(currentConfiguration());
}

void GeneratorFactoryBase::initVariables()
{
	mVariables = new parts::Variables(pathToTemplates(), mRobotModelManager.model(), mLuaTranslator.toolbox());
}

void GeneratorFactoryBase::initSubprograms()
{
	mSubprograms = new parts::Subprograms(mRepo, mErrorReporter
			, pathToTemplates(), nameNormalizerConverter());
}

void GeneratorFactoryBase::initEngines()
{
	mEngines = new parts::Engines(pathToTemplates(), outputPortConverter(), enginesConverter());
}

void GeneratorFactoryBase::initSensors()
{
	mSensors = new parts::Sensors(pathToTemplates(), inputPortConverter());
}

void GeneratorFactoryBase::initFunctions()
{
	mFunctions = new parts::Functions(pathToTemplates());
}

void GeneratorFactoryBase::initImages()
{
	mImages = new parts::Images(pathToTemplates());
}

void GeneratorFactoryBase::initDeviceVariables()
{
	mDeviceVariables = new parts::DeviceVariables();
}

QList<parts::InitTerminateCodeGenerator *> GeneratorFactoryBase::initTerminateGenerators()
{
	return QList<parts::InitTerminateCodeGenerator *>()
			<< engines() << sensors() << functions();
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

parts::Threads &GeneratorFactoryBase::threads()
{
	return *mThreads;
}

parts::Engines *GeneratorFactoryBase::engines()
{
	return mEngines;
}

parts::Sensors *GeneratorFactoryBase::sensors()
{
	return mSensors;
}

parts::Functions *GeneratorFactoryBase::functions()
{
	return mFunctions;
}

parts::Images *GeneratorFactoryBase::images()
{
	return mImages;
}

parts::DeviceVariables *GeneratorFactoryBase::deviceVariables() const
{
	return mDeviceVariables;
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

AbstractSimpleGenerator *GeneratorFactoryBase::switchHeadGenerator(Id const &id
		, GeneratorCustomizer &customizer, QStringList const &values)
{
	return new SwitchGenerator(mRepo, customizer, id, "head", values, this);
}

AbstractSimpleGenerator *GeneratorFactoryBase::switchMiddleGenerator(Id const &id
		, GeneratorCustomizer &customizer, QStringList const &values)
{
	return new SwitchGenerator(mRepo, customizer, id, "middle", values, this);
}

AbstractSimpleGenerator *GeneratorFactoryBase::switchDefaultGenerator(Id const &id
		, GeneratorCustomizer &customizer)
{
	return new SwitchGenerator(mRepo, customizer, id, "default", {}, this);
}

AbstractSimpleGenerator *GeneratorFactoryBase::forkCallGenerator(Id const &id
		, GeneratorCustomizer &customizer, IdList const &threads)
{
	return new ForkCallGenerator(mRepo, customizer, id, threads, this);
}

AbstractSimpleGenerator *GeneratorFactoryBase::simpleGenerator(qReal::Id const &id
		, GeneratorCustomizer &customizer)
{
	QString const elementType = id.element();

	if (elementType == "CommentBlock") {
		return new CommentElementGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Function") {
		return new FunctionElementGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("EnginesForward") || elementType.contains("EnginesBackward")) {
		return new EnginesGenerator(mRepo, customizer, id, elementType, this);
	} else if (elementType.contains("EnginesStop")) {
		return new EnginesStopGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Timer") {
		return new TimerGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("Beep")) {
		return new BeepGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("PlayTone")) {
		return new PlayToneGenerator(mRepo, customizer, id, this);
	} else if (elementType == "InitialNode") {
		return new InitialNodeGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("ClearEncoder")) {
		return new NullificationEncoderGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("WaitForColor")) {
		return new WaitForColorBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("WaitForColorIntensity")) {
		return new WaitForColorIntensityBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("WaitForLight")) {
		return new WaitForLightBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("WaitForTouchSensor")) {
		return new WaitForTouchSensorBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("WaitForSonarDistance")) {
		return new WaitForSonarBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("WaitForEncoder")) {
		return new WaitForEncoderBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("WaitForSound")) {
		return new WaitForSoundBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("WaitForGyroscope")) {
		return new WaitForGyroscopeBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("WaitForAccelerometer")) {
		return new WaitForAccelerometerBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("DrawPixel")) {
		return new DrawPixelBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("DrawLine")) {
		return new DrawLineBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("DrawCircle")) {
		return new DrawCircleBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("PrintText")) {
		return new PrintTextBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("DrawRect")) {
		return new DrawRectBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("ClearScreen")) {
		return new ClearScreenBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Subprogram") {
		return new SubprogramsSimpleGenerator(mRepo, customizer, id, this);
	} else if (elementType == "VariableInit") {
		return new VariableInitGenerator(mRepo, customizer, id, this);
	}

	return new NullGenerator(mRepo, customizer, id, this);
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

AbstractSimpleGenerator *GeneratorFactoryBase::labelGenerator(qReal::Id const &id
		, GeneratorCustomizer &customizer)
{
	return new LabelGenerator(mRepo, customizer, id, this);
}

AbstractSimpleGenerator *GeneratorFactoryBase::gotoSimpleGenerator(qReal::Id const &id
		, GeneratorCustomizer &customizer)
{
	return new GotoSimpleGenerator(mRepo, customizer, id, this);
}

AbstractSimpleGenerator *GeneratorFactoryBase::finalNodeGenerator(qReal::Id const &id
		, GeneratorCustomizer &customizer, bool fromMainGenerator)
{
	return new FinalNodeGenerator(mRepo, customizer, id, fromMainGenerator, this);
}

// Converters can be instantiated without taking ownership because binders do this

Binding::ConverterInterface *GeneratorFactoryBase::intPropertyConverter(Id const &block) const
{
	return new converters::IntPropertyConverter(pathToTemplates(), mLuaTranslator, block
			, reservedVariableNameConverter(), typeConverter());
}

Binding::ConverterInterface *GeneratorFactoryBase::floatPropertyConverter(Id const &block) const
{
	return new converters::FloatPropertyConverter(mLuaTranslator, block, reservedVariableNameConverter());
}

Binding::ConverterInterface *GeneratorFactoryBase::boolPropertyConverter(Id const &block, bool needInverting) const
{
	return new converters::BoolPropertyConverter(pathToTemplates(), mLuaTranslator
			, block, reservedVariableNameConverter(), needInverting);
}

Binding::ConverterInterface *GeneratorFactoryBase::stringPropertyConverter() const
{
	return new converters::StringPropertyConverter;
}

Binding::ConverterInterface *GeneratorFactoryBase::reservedVariableNameConverter() const
{
	return new converters::ReservedVariablesConverter(pathToTemplates()
			, mErrorReporter
			, mRobotModelManager.model()
			, currentConfiguration()
			, inputPortConverter()
			, *deviceVariables());
}

Binding::ConverterInterface *GeneratorFactoryBase::nameNormalizerConverter() const
{
	return new converters::NameNormalizerConverter;
}

Binding::ConverterInterface *GeneratorFactoryBase::functionBlockConverter(qReal::Id const &block) const
{
	return new converters::FunctionBlockConverter(mLuaTranslator, block, reservedVariableNameConverter());
}

Binding::ConverterInterface *GeneratorFactoryBase::inequalitySignConverter() const
{
	return new converters::InequalitySignConverter(pathToTemplates());
}

Binding::MultiConverterInterface *GeneratorFactoryBase::enginesConverter() const
{
	return new converters::EnginePortsConverter(outputPortConverter());
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

Binding::ConverterInterface *GeneratorFactoryBase::switchConditionsMerger(QStringList const &values) const
{
	return new converters::SwitchConditionsMerger(pathToTemplates(), reservedVariableNameConverter(), values);
}

QString GeneratorFactoryBase::initCode()
{
	QStringList result;
	QList<parts::InitTerminateCodeGenerator *> const generators(initTerminateGenerators());
	foreach (parts::InitTerminateCodeGenerator *generator, generators) {
		result << generator->initCode();
	}

	return result.join('\n');
}

QString GeneratorFactoryBase::terminateCode()
{
	QStringList result;
	QList<parts::InitTerminateCodeGenerator *> const generators(initTerminateGenerators());
	foreach (parts::InitTerminateCodeGenerator *generator, generators) {
		result << generator->terminateCode();
	}

	return result.join('\n');
}

QString GeneratorFactoryBase::isrHooksCode()
{
	QStringList result;
	QList<parts::InitTerminateCodeGenerator *> const generators(initTerminateGenerators());
	foreach (parts::InitTerminateCodeGenerator *generator, generators) {
		result << generator->isrHooksCode();
	}

	return result.join('\n');
}

QMap<PortInfo, DeviceInfo> GeneratorFactoryBase::currentConfiguration() const
{
	Id const logicalId = mRepo.logicalId(mDiagram);
	QString const configuration = mRepo.property(logicalId, "devicesConfiguration").toString();
	QMap<PortInfo, DeviceInfo> result = RobotModelUtils::deserialize(configuration)[mRobotModelManager.model().name()];
	// At the moment we have sensors configuration from widget-configurer. We must also add here non-configurable
	// by user devices (like encoders, displays and so on).
	for (PortInfo const &port : mRobotModelManager.model().availablePorts()) {
		if (!mRobotModelManager.model().configurablePorts().contains(port)
				&& mRobotModelManager.model().allowedDevices(port).count() == 1
				&& port.direction() == input) {
			result[port] = mRobotModelManager.model().allowedDevices(port)[0];
		}
	}

	return result;
}
