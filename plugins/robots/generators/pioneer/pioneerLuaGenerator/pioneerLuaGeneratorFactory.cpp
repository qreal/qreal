/* Copyright 2017 QReal Research Group
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

#include "pioneerLuaGeneratorFactory.h"

#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>

#include "parts/ledPart.h"
#include "parts/tofPart.h"
#include "parts/magnetPart.h"
#include "parts/randomGeneratorPart.h"
#include "generators/gotoLabelManager.h"
#include "simpleGenerators/endOfHandlerGenerator.h"
#include "simpleGenerators/geoLandingGenerator.h"
#include "simpleGenerators/geoTakeoffGenerator.h"
#include "simpleGenerators/gotoGenerator.h"
#include "simpleGenerators/goToPointGenerator.h"
#include "simpleGenerators/goToGPSPointGenerator.h"
#include "simpleGenerators/pioneerGetLPSPosition.h"
#include "simpleGenerators/initialNodeGenerator.h"
#include "simpleGenerators/labelGenerator.h"
#include "simpleGenerators/pioneerMagnetGenerator.h"
#include "simpleGenerators/pioneerPrintGenerator.h"
#include "simpleGenerators/pioneerSystemGenerator.h"
#include "simpleGenerators/pioneerLedGenerator.h"
#include "simpleGenerators/pioneerYawGenerator.h"
#include "simpleGenerators/randomInitGenerator.h"
#include "simpleGenerators/pioneerReadRangeSensor.h"

using namespace pioneer::lua;
using namespace generatorBase::simple;

PioneerLuaGeneratorFactory::PioneerLuaGeneratorFactory(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName
		, GotoLabelManager &gotoLabelManager)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
	, mGeneratorName(generatorName)
	, mGotoLabelManager(gotoLabelManager)
{
}

PioneerLuaGeneratorFactory::~PioneerLuaGeneratorFactory()
{
}

generatorBase::simple::AbstractSimpleGenerator *PioneerLuaGeneratorFactory::simpleGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer)
{
	const QString elementType = id.element();
	if (elementType == "EndOfHandler") {
		return new EndOfHandlerGenerator(mRepo, customizer, id, this);
	} else if (elementType == "InitialNode") {
		qReal::Id firstId = mRepo.to(mRepo.links(id).first());
		QString localLabelFor = mGotoLabelManager.labelFor(firstId);
		return new InitialNodeGenerator(mRepo, customizer, firstId, this);
	} else if (elementType == "GeoTakeoff") {
		return new GeoTakeoffGenerator(mRepo, customizer, id, this);
	} else if (elementType == "GeoLanding") {
		return new GeoLandingGenerator(mRepo, customizer, id, this);
	} else if (elementType == "GoToPoint") {
		return new GoToPointGenerator(mRepo, customizer, id, this);
	} else if (elementType == "GoToGPSPoint") {
		return new GoToGPSPointGenerator(mRepo, customizer, id, this);
	} else if (elementType == "PioneerGetLPSPosition") {
		return new PioneerGetLPSPosition(mRepo, customizer, id, this);
	} else if (elementType == "PioneerMagnet") {
		return new PioneerMagnetGenerator(mRepo, customizer, id, this);
	} else if (elementType == "PioneerPrint") {
		return new PioneerPrintGenerator(mRepo, customizer, id, this);
	} else if (elementType == "PioneerSystem") {
		return new PioneerSystemGenerator(mRepo, customizer, id, this);
	} else if (elementType == "PioneerLed") {
		return new PioneerLedGenerator(mRepo, customizer, id, this);
	} else if (elementType == "PioneerYaw") {
		return new PioneerYawGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Randomizer") {
		return new RandomInitGenerator(mRepo, customizer, id, this);
	} else if (elementType == "PioneerReadRangeSensor") {
		return new PioneerReadRangeSensor(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QStringList PioneerLuaGeneratorFactory::pathsToTemplates() const
{
	return {":/" + mGeneratorName + "/templates"};
}

void PioneerLuaGeneratorFactory::initialize()
{
	generatorBase::GeneratorFactoryBase::initialize();
	mLedPart.reset(new LedPart(pathsToTemplates()));
	mTofPart.reset(new TofPart(pathsToTemplates()));
	mMagnetPart.reset(new MagnetPart(pathsToTemplates()));
	mRandomGeneratorPart.reset(new RandomGeneratorPart(pathsToTemplates()));
}

LedPart& PioneerLuaGeneratorFactory::ledPart()
{
	return *mLedPart;
}

TofPart &PioneerLuaGeneratorFactory::tofPart()
{
	return *mTofPart;
}

MagnetPart& PioneerLuaGeneratorFactory::magnetPart()
{
	return *mMagnetPart;
}

RandomGeneratorPart& PioneerLuaGeneratorFactory::randomGeneratorPart()
{
	return *mRandomGeneratorPart;
}

generatorBase::simple::AbstractSimpleGenerator *PioneerLuaGeneratorFactory::labelGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer)
{
	return new LabelGenerator(mRepo, customizer, id, this, mGotoLabelManager);
}

generatorBase::simple::AbstractSimpleGenerator *PioneerLuaGeneratorFactory::gotoSimpleGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer)
{
	return new GotoGenerator(mRepo, customizer, id, this, mGotoLabelManager);
}

QList<generatorBase::parts::InitTerminateCodeGenerator *> PioneerLuaGeneratorFactory::initTerminateGenerators()
{
	auto result = generatorBase::GeneratorFactoryBase::initTerminateGenerators();
	result << mRandomGeneratorPart.data() << mMagnetPart.data() << mLedPart.data() << mTofPart.data();
	return result;
}
