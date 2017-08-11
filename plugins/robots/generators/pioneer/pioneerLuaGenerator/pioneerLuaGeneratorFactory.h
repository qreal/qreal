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

#pragma once

#include <generatorBase/generatorFactoryBase.h>

namespace pioneer {
namespace lua {

class GotoLabelManager;

/// Factory that creates simple generators for Pioneer-specific blocks.
class PioneerLuaGeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	PioneerLuaGeneratorFactory(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, const QString &generatorName
			, GotoLabelManager &gotoLabelManager);

	~PioneerLuaGeneratorFactory() override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QStringList pathsToTemplates() const override;

private:
	/// Returns goto label generator that can generate human-readable labels based no block types.
	generatorBase::simple::AbstractSimpleGenerator *labelGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	/// Returns goto generator that uses labels provided by goto label generator.
	generatorBase::simple::AbstractSimpleGenerator *gotoSimpleGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	/// Generator name is used as a prefix to a path to templates in resources.
	const QString mGeneratorName;

	/// Storage and generator for human-readable goto labels. Used in label ganerator and goto generator created by
	/// this factory.
	GotoLabelManager &mGotoLabelManager;
};

}
}
