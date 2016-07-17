/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "trikGeneratorFactory.h"

#include <generatorBase/converters/regexpMultiConverter.h>
#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>
#include <generatorBase/lua/luaProcessor.h>

#include "simpleGenerators/detectGenerator.h"
#include "simpleGenerators/detectorToVariableGenerator.h"
#include "simpleGenerators/drawLineGenerator.h"
#include "simpleGenerators/drawPixelGenerator.h"
#include "simpleGenerators/drawRectGenerator.h"
#include "simpleGenerators/drawEllipseGenerator.h"
#include "simpleGenerators/drawArcGenerator.h"
#include "simpleGenerators/initCameraGenerator.h"
#include "simpleGenerators/initVideoStreamingGenerator.h"
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
#include "simpleGenerators/waitGamepadButtonGenerator.h"
#include "simpleGenerators/waitGamepadConnectGenerator.h"
#include "simpleGenerators/waitGamepadDisconnectGenerator.h"
#include "simpleGenerators/waitGamepadWheelGenerator.h"
#include "simpleGenerators/waitPadPressGenerator.h"
#include "simpleGenerators/fileWriteGenerator.h"
#include "simpleGenerators/removeFileGenerator.h"
#include "parts/trikDeviceVariables.h"

using namespace trik;
using namespace trik::simple;
using namespace generatorBase;
using namespace generatorBase::simple;

TrikGeneratorFactory::TrikGeneratorFactory(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, lua::LuaProcessor &luaProcessor
		, const QStringList &pathsToTemplates)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
	, mPathsToTemplates(pathsToTemplates)
{
}

TrikGeneratorFactory::~TrikGeneratorFactory()
{
}

AbstractSimpleGenerator *TrikGeneratorFactory::simpleGenerator(const qReal::Id &id
		, GeneratorCustomizer &customizer)
{
	const QString elementType = id.element();
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
	} else if (elementType == "TrikInitVideoStreaming") {
		return new InitVideoStreamingGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitForMotion") {
		return new WaitForMotionGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitForIRDistance") {
		return new WaitForInfraredSensorGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitGamepadButton") {
		return new WaitGamepadButtonGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitPadPress") {
		return new WaitPadPressGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitGamepadWheel") {
		return new WaitGamepadWheelGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitGamepadConnect") {
		return new WaitGamepadConnectGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWaitGamepadDisconnect") {
		return new WaitGamepadDisconnectGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikWriteToFile") {
		return new FileWriteGenerator(mRepo, customizer, id, this);
	} else if (elementType == "TrikRemoveFile") {
		return new RemoveFileGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QStringList TrikGeneratorFactory::pathsToTemplates() const
{
	return mPathsToTemplates; //{":/" + mGeneratorName + "/templates"};
}

generatorBase::parts::DeviceVariables *TrikGeneratorFactory::deviceVariables() const
{
	return new trik::parts::TrikDeviceVariables();
}
