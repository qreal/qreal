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
	Ev3GeneratorFactory(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, QString const &generatorName);

	~Ev3GeneratorFactory() override;

	/// Returns a pointer to a code generator for blocks with if semantics
	generatorBase::simple::AbstractSimpleGenerator *ifGenerator(qReal::Id const &id
			, generatorBase::GeneratorCustomizer &customizer, bool elseIsEmpty, bool needInverting) override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(qReal::Id const &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	generatorBase::simple::Binding::ConverterInterface *outputPortNameConverter() const;

	generatorBase::simple::Binding::ConverterInterface *goToBlockNumberConverter() const;

	QString pathToTemplates() const override;

private:
	QString const mGeneratorName;
	int mGoToBlockNumber;
};

}

