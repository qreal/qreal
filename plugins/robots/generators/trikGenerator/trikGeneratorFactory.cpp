#include "trikGeneratorFactory.h"
#include <generatorBase/converters/regexpMultiConverter.h>
#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>
#include "converters/engineV4PortConverter.h"
#include "converters/engineV6PortConverter.h"
#include "converters/encoderV4PortConverter.h"
#include "converters/encoderV6PortConverter.h"
#include "converters/trikStringPropertyConverter.h"
#include "simpleGenerators/detectLineGenerator.h"
#include "simpleGenerators/initCameraGenerator.h"
#include "simpleGenerators/ledGenerator.h"
#include "simpleGenerators/lineDetectorToVariableGenerator.h"
#include "simpleGenerators/playToneGenerator.h"
#include "simpleGenerators/drawLineGenerator.h"
#include "simpleGenerators/drawPixelGenerator.h"
#include "simpleGenerators/drawRectGenerator.h"
#include "simpleGenerators/drawEllipseGenerator.h"
#include "simpleGenerators/drawArcGenerator.h"
#include "simpleGenerators/setPainterWidthGenerator.h"
#include "simpleGenerators/setPainterColorGenerator.h"
#include "simpleGenerators/sadSmileGenerator.h"
#include "simpleGenerators/sayGenerator.h"
#include "simpleGenerators/setBackgroundGenerator.h"
#include "simpleGenerators/smileGenerator.h"
#include "simpleGenerators/systemGenerator.h"
#include "simpleGenerators/trikEnginesGenerator.h"
#include "simpleGenerators/trikEnginesStopGenerator.h"
#include "simpleGenerators/waitForInfraredSensorGenerator.h"
#include "simpleGenerators/waitForMotionGenerator.h"
#include "simpleGenerators/trikNullificationEncoderGenerator.h"
#include "parts/trikVariables.h"
#include "parts/trikDeviceVariables.h"

using namespace trik;
using namespace trik::simple;
using namespace generatorBase;
using namespace generatorBase::simple;

TrikGeneratorFactory::TrikGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager)
{
}

TrikGeneratorFactory::~TrikGeneratorFactory()
{
}

AbstractSimpleGenerator *TrikGeneratorFactory::simpleGenerator(qReal::Id const &id
		, GeneratorCustomizer &customizer)
{
	QString const elementType = id.element();
	if (elementType.contains("EnginesForward")
			|| elementType.contains("EnginesBackward")
			|| elementType.contains("AngularServo"))
	{
		return new TrikEnginesGenerator(mRepo, customizer, id, elementType, this);
	} else if (elementType.contains("EnginesStop")) {
		return new TrikEnginesStopGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("ClearEncoder")) {
		return new TrikNullificationEncoderGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("TrikPlayTone")) {
		return new PlayToneGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikDrawLine") {
		return new DrawLineGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikDrawPixel") {
		return new DrawPixelGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikDrawRect") {
		return new DrawRectGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikDrawEllipse") {
		return new DrawEllipseGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikDrawArc") {
		return new DrawArcGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikSetPainterWidth") {
		return new SetPainterWidthGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikSetPainterColor") {
		return new SetPainterColorGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikSmile") {
		return new SmileGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikSadSmile") {
		return new SadSmileGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikSay") {
		return new SayGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikSetBackground") {
		return new SetBackgroundGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikSystem") {
		return new SystemGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikLed") {
		return new LedGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikDetectLine") {
		return new DetectLineGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikInitCamera") {
		return new InitCameraGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikLineDetectorToVariable") {
		return new LineDetectorToVariableGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitForEnter") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForEnter.t", this);
	} else if (elementType == "TrikWaitForLeft") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForLeft.t", this);
	} else if (elementType == "TrikWaitForRight") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForRight.t", this);
	} else if (elementType == "TrikWaitForUp") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForUp.t", this);
	} else if (elementType == "TrikWaitForDown") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForDown.t", this);
	} else if (elementType == "TrikWaitForPower") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForPower.t", this);
	} else if (elementType == "TrikWaitForMotion") {
		return new WaitForMotionGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitForIRDistance") {
		return new WaitForInfraredSensorGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QString TrikGeneratorFactory::pathToTemplates() const
{
	return ":/trik/templates";
}

Binding::MultiConverterInterface *TrikGeneratorFactory::enginesConverter() const
{
	return new generatorBase::converters::RegexpMultiConverter(motorPortSplitRegexp(), motorPortConverter());
}

Binding::MultiConverterInterface *TrikGeneratorFactory::encodersConverter() const
{
	return new generatorBase::converters::RegexpMultiConverter(motorPortSplitRegexp(), encoderPortConverter());
}

Binding::ConverterInterface *TrikGeneratorFactory::inputPortConverter() const
{
	return new Binding::EmptyConverter;
}

Binding::ConverterInterface *TrikGeneratorFactory::outputPortConverter() const
{
	return encoderPortConverter();
}

generatorBase::simple::Binding::ConverterInterface *TrikGeneratorFactory::stringPropertyConverter() const
{
	return new converters::TrikStringPropertyConverter(*mVariables);
}

void TrikGeneratorFactory::initVariables()
{
	mVariables = new parts::TrikVariables(pathToTemplates(), mRobotModelManager.model());
}

Binding::ConverterInterface *TrikGeneratorFactory::motorPortConverter() const
{
	if (mRobotModelManager.model().name().contains("V4")) {
		return new converters::PowerV4MotorPortConverter;
	} else if (mRobotModelManager.model().name().contains("V6")) {
		return new converters::PowerV6MotorPortConverter;
	}

	/// @todo: Inconsistent scenario
	return new converters::PowerV6MotorPortConverter;
}

Binding::ConverterInterface *TrikGeneratorFactory::encoderPortConverter() const
{
	if (mRobotModelManager.model().name().contains("V4")) {
		return new converters::EncoderV4PortConverter;
	} else if (mRobotModelManager.model().name().contains("V6")) {
		return new converters::EncoderV6PortConverter;
	}

	/// @todo: Inconsistent scenario
	return new converters::EncoderV6PortConverter;
}

QString TrikGeneratorFactory::motorPortSplitRegexp() const
{
	if (mRobotModelManager.model().name().contains("V4")) {
		return converters::PowerV4MotorPortConverter::splitRegexp();
	} else if (mRobotModelManager.model().name().contains("V6")) {
		return converters::PowerV6MotorPortConverter::splitRegexp();
	}

	/// @todo: Inconsistent scenario
	return converters::PowerV6MotorPortConverter::splitRegexp();
}

generatorBase::parts::DeviceVariables *TrikGeneratorFactory::deviceVariables() const
{
	return new trik::parts::TrikDeviceVariables();
}
