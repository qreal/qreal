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
#include "simpleGenerators/waitForButtonGenerator.h"
#include "simpleGenerators/waitForTouchSensorGenerator.h"
#include "simpleGenerators/ifElementGenerator.h"

#include "converters/outputPortNameConverter.h"
#include "converters/goToBlockNumberConverter.h"

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
	, mGoToBlockNumber(0)
{
}

Ev3GeneratorFactory::~Ev3GeneratorFactory()
{
}

AbstractSimpleGenerator *Ev3GeneratorFactory::ifGenerator(const qReal::Id &id, generatorBase::GeneratorCustomizer &customizer, bool elseIsEmpty, bool needInverting)
{
	mGoToBlockNumber++;
	return new IfElementGenerator(mRepo, customizer, id, elseIsEmpty, needInverting, this);
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
	return new OutputPortNameConverter(pathsToTemplates());
}

Binding::ConverterInterface *Ev3GeneratorFactory::goToBlockNumberConverter() const
{
	return new GoToBlockNumberConverter(QString::number(mGoToBlockNumber));
}

QStringList Ev3GeneratorFactory::pathsToTemplates() const
{
	return { ":/" + mGeneratorName + "/templates" };
}
