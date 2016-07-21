/* Copyright 2015 CyberTech Labs Ltd.
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

#include <generatorBase/generatorCustomizer.h>

#include "ev3RbfGeneratorFactory.h"

namespace ev3 {
namespace rbf {

class Ev3RbfGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	Ev3RbfGeneratorCustomizer(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, const QString &generatorName);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	Ev3RbfGeneratorFactory mFactory;
};

}
}
