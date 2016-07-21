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

#include <ev3GeneratorBase/ev3GeneratorFactory.h>

namespace ev3 {
namespace rbf {

/// Generator factory adding ability to prepend code to blocks.
class Ev3RbfGeneratorFactory : public Ev3GeneratorFactory
{
public:
	Ev3RbfGeneratorFactory(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, const QString &generatorName);

	generatorBase::simple::AbstractSimpleGenerator *labelGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	/// Returns a list of lines of code prepended to some block`s generated code.
	QStringList additionalCode(const qReal::Id &id) const;

	/// Prependeds some block`s generated code with given.
	void addAdditionalCode(const qReal::Id &id, const QStringList &code);

private:
	QMap<qReal::Id, QStringList> mAdditionalCode;
};

}
}
