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

#include "simpleGenerators/geoLandingGenerator.h"
#include "simpleGenerators/geoTakeoffGenerator.h"
#include "simpleGenerators/goToPointGenerator.h"
#include "simpleGenerators/initialNodeGenerator.h"

using namespace pioneer::lua;
using namespace generatorBase::simple;

PioneerLuaGeneratorFactory::PioneerLuaGeneratorFactory(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
	, mGeneratorName(generatorName)
{
}

PioneerLuaGeneratorFactory::~PioneerLuaGeneratorFactory()
{
}

generatorBase::simple::AbstractSimpleGenerator *PioneerLuaGeneratorFactory::simpleGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer)
{
	const QString elementType = id.element();
	if (elementType == "InitialNode") {
		return new InitialNodeGenerator(mRepo, customizer, id, this);
	} else if (elementType == "GeoTakeoff") {
		return new GeoTakeoffGenerator(mRepo, customizer, id, this);
	} else if (elementType == "GeoLanding") {
		return new GeoLandingGenerator(mRepo, customizer, id, this);
	} else if (elementType == "GoToPoint") {
		return new GoToPointGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QStringList PioneerLuaGeneratorFactory::pathsToTemplates() const
{
	return {":/" + mGeneratorName + "/templates"};
}
