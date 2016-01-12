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

#pragma once

#include <generatorBase/generatorFactoryBase.h>

#include "ev3GeneratorBase/ev3GeneratorBaseDeclSpec.h"

namespace ev3 {

class ROBOTS_EV3_GENERATOR_BASE_EXPORT Ev3GeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	Ev3GeneratorFactory(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, const QString &generatorName);

	~Ev3GeneratorFactory() override;

	generatorBase::simple::AbstractSimpleGenerator *ifGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer, bool elseIsEmpty, bool needInverting) override;

	generatorBase::simple::AbstractSimpleGenerator *forLoopGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	/// Converts an output port with given id into generator-dependent port representation.
	generatorBase::simple::Binding::ConverterInterface *outputPortNameConverter() const;

	/// Converts TRIK Studio LED color enum values to corresponding names in EV3 runtime.
	generatorBase::simple::Binding::ConverterInterface *ledColorConverter() const;

	QStringList pathsToTemplates() const override;

private:
	const QString mGeneratorName;
};

}

