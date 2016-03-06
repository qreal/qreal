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
{
}

Ev3GeneratorFactory::~Ev3GeneratorFactory()
{
}

AbstractSimpleGenerator *Ev3GeneratorFactory::ifGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer, bool elseIsEmpty, bool needInverting)
{
	return randomIdGenerator(GeneratorFactoryBase::ifGenerator(id, customizer, elseIsEmpty, needInverting));
}

AbstractSimpleGenerator *Ev3GeneratorFactory::forLoopGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer)
{
	return randomIdGenerator(GeneratorFactoryBase::forLoopGenerator(id, customizer));
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
	} else if (elementType == "Ev3Led" ) {
		return new LedGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Ev3ClearEncoder" ) {
		return new NullificationEncoderGenerator(mRepo, customizer, id, this);
	}

	return randomIdGenerator(GeneratorFactoryBase::simpleGenerator(id, customizer));
}

Binding::ConverterInterface *Ev3GeneratorFactory::outputPortNameConverter() const
{
	return new OutputPortNameConverter(pathsToTemplates());
}

Binding::ConverterInterface *Ev3GeneratorFactory::ledColorConverter() const
{
	return new LedColorConverter(pathsToTemplates());
}

QStringList Ev3GeneratorFactory::pathsToTemplates() const
{
	return { ":/" + mGeneratorName + "/templates" };
}
