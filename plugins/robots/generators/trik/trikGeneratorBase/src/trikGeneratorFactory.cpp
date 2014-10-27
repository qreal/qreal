#include "trikGeneratorFactory.h"

#include <generatorBase/converters/regexpMultiConverter.h>
#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>
#include <generatorBase/lua/luaProcessor.h>

#include "converters/trikStringPropertyConverter.h"
#include "simpleGenerators/detectGenerator.h"
#include "simpleGenerators/detectorToVariableGenerator.h"
#include "simpleGenerators/drawLineGenerator.h"
#include "simpleGenerators/drawPixelGenerator.h"
#include "simpleGenerators/drawRectGenerator.h"
#include "simpleGenerators/drawEllipseGenerator.h"
#include "simpleGenerators/drawArcGenerator.h"
#include "simpleGenerators/initCameraGenerator.h"
#include "simpleGenerators/ledGenerator.h"
#include "simpleGenerators/playToneGenerator.h"
#include "simpleGenerators/waitForMessageGenerator.h"
#include "simpleGenerators/sadSmileGenerator.h"
#include "simpleGenerators/sayGenerator.h"
#include "simpleGenerators/sendMessageGenerator.h"
#include "simpleGenerators/setPainterWidthGenerator.h"
#include "simpleGenerators/setPainterColorGenerator.h"
#include "simpleGenerators/setBackgroundGenerator.h"
#include "simpleGenerators/smileGenerator.h"
#include "simpleGenerators/systemGenerator.h"
#include "simpleGenerators/trikEnginesGenerator.h"
#include "simpleGenerators/waitForInfraredSensorGenerator.h"
#include "simpleGenerators/waitForMotionGenerator.h"
#include "parts/trikVariables.h"
#include "parts/trikDeviceVariables.h"

using namespace trik;
using namespace trik::simple;
using namespace generatorBase;
using namespace generatorBase::simple;

TrikGeneratorFactory::TrikGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, lua::LuaProcessor &luaProcessor
		, QString const &generatorName)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
	, mGeneratorName(generatorName)
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
	} else if (elementType == "TrikSendMessage") {
		return new SendMessageGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitForMessage") {
		return new WaitForMessageGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikSetBackground") {
		return new SetBackgroundGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikSystem") {
		return new SystemGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikLed") {
		return new LedGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikDetect") {
		return new DetectGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikInitCamera") {
		return new InitCameraGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikDetectorToVariable") {
		return new DetectorToVariableGenerator(mRepo, customizer, id, this);
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
	return ":/" + mGeneratorName + "/templates";
}

generatorBase::simple::Binding::ConverterInterface *TrikGeneratorFactory::stringPropertyConverter() const
{
	return new converters::TrikStringPropertyConverter(mGeneratorName, *mVariables, *reservedVariableNameConverter());
}

void TrikGeneratorFactory::initVariables()
{
	mVariables = new parts::TrikVariables(pathToTemplates(), mRobotModelManager.model(), mLuaTranslator.toolbox());
}

generatorBase::parts::DeviceVariables *TrikGeneratorFactory::deviceVariables() const
{
	return new trik::parts::TrikDeviceVariables();
}
