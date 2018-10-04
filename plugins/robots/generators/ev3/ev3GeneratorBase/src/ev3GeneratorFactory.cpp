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

#include "ev3GeneratorBase/ev3GeneratorFactory.h"

#include "simpleGenerators/drawPixelGenerator.h"
#include "simpleGenerators/drawLineGenerator.h"
#include "simpleGenerators/drawRectGenerator.h"
#include "simpleGenerators/drawCircleGenerator.h"
#include "simpleGenerators/enginesGenerator.h"
#include "simpleGenerators/enginesStopGenerator.h"
#include "simpleGenerators/ledGenerator.h"
#include "simpleGenerators/nullificationEncoderGenerator.h"
#include "simpleGenerators/sendMailGenerator.h"
#include "simpleGenerators/receiveMailGenerator.h"
#include "simpleGenerators/calibrateGyroscopeGenerator.h"
#include "simpleGenerators/startCompassCalibrationGenerator.h"
#include "simpleGenerators/stopCompassCalibrationGenerator.h"
#include "simpleGenerators/readRGBGenerator.h"

#include "simpleGenerators/lineLeader/calibrateBlackGenerator.h"
#include "simpleGenerators/lineLeader/calibratePIDGenerator.h"
#include "simpleGenerators/lineLeader/calibrateWhiteGenerator.h"
#include "simpleGenerators/lineLeader/readAverageGenerator.h"
#include "simpleGenerators/lineLeader/readSensorArrayGenerator.h"
#include "simpleGenerators/lineLeader/readSteeringGenerator.h"
#include "simpleGenerators/lineLeader/sleepGenerator.h"
#include "simpleGenerators/lineLeader/wakeUpGenerator.h"


#include "converters/outputPortNameConverter.h"
#include "converters/ledColorConverter.h"

using namespace ev3;
using namespace ev3::simple;
using namespace ev3::converters;
using namespace generatorBase::simple;

Ev3GeneratorFactory::Ev3GeneratorFactory(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
	, mGeneratorName(generatorName)
	, mMailboxes({":/" + mGeneratorName + "/templates"})
{
}

Ev3GeneratorFactory::~Ev3GeneratorFactory()
{
}

parts::Mailboxes &Ev3GeneratorFactory::mailboxes()
{
	return mMailboxes;
}

AbstractSimpleGenerator *Ev3GeneratorFactory::ifGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer, bool elseIsEmpty, bool needInverting)
{
	return GeneratorFactoryBase::ifGenerator(id, customizer, elseIsEmpty, needInverting);
}

AbstractSimpleGenerator *Ev3GeneratorFactory::forLoopGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer)
{
	return GeneratorFactoryBase::forLoopGenerator(id, customizer);
}

generatorBase::simple::AbstractSimpleGenerator *Ev3GeneratorFactory::simpleGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer)
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
	} else if (elementType == "Ev3Led") {
		return new LedGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3ClearEncoder") {
		return new NullificationEncoderGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3SendMail") {
		return new SendMailGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3WaitForReceivingMail") {
		return new ReceiveMailGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3CalibrateGyroscope") {
		return new CalibrateGyroscopeGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3StartCompassCalibration") {
		return new StartCompassCalibrationGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3StopCompassCalibration") {
		return new StopCompassCalibrationGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3ReadRGB") {
		return new ReadRGBGenerator(mRepo, customizer, id, this);
	}

	else if (elementType == "Ev3CalibrateWhiteLL") {
		return new lineLeader::CalibrateWhiteGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3CalibrateBlackLL") {
		return new lineLeader::CalibrateBlackGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3CalibratePIDLL") {
		return new lineLeader::CalibratePIDGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3SleepLL") {
		return new lineLeader::SleepGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3WakeUpLL") {
		return new lineLeader::WakeUpGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3ReadAvrLL") {
		return new lineLeader::ReadAverageGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3ReadAllLL") {
		return new lineLeader::ReadSensorArrayGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3ReadSteeringLL") {
		return new lineLeader::ReadSteeringGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

Binding::ConverterInterface *Ev3GeneratorFactory::outputPortNameConverter() const
{
	return new OutputPortNameConverter(pathsToTemplates()
			, mRobotModelManager.model().availablePorts());
}

Binding::ConverterInterface *Ev3GeneratorFactory::ledColorConverter() const
{
	return new LedColorConverter(pathsToTemplates());
}

QStringList Ev3GeneratorFactory::pathsToTemplates() const
{
	return { ":/" + mGeneratorName + "/templates" };
}

void Ev3GeneratorFactory::reportError(const QString &errorMessage, const qReal::Id &id)
{
	mErrorReporter.addError(errorMessage, id);
}
