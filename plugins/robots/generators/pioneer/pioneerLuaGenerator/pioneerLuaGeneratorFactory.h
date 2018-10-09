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
class LedPart;
class TofPart;
class MagnetPart;
class RandomGeneratorPart;

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

	void initialize() override;

	/// Returns generator part that analyzes LED usage and initializes it if it is needed.
	LedPart& ledPart();

	/// Returns generator part that analyzes Tof usage and initializes it if it is needed.
	TofPart& tofPart();

	/// Returns generator part that analyzes magnet usage and initializes it if it is needed.
	MagnetPart& magnetPart();

	/// Returns generator part that analyzes RNG usage in a program and initializes it if it is needed.
	RandomGeneratorPart& randomGeneratorPart();

private:
	/// Returns goto label generator that can generate human-readable labels based no block types.
	generatorBase::simple::AbstractSimpleGenerator *labelGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	/// Returns goto generator that uses labels provided by goto label generator.
	generatorBase::simple::AbstractSimpleGenerator *gotoSimpleGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	/// Adds random number generator initialization part to a list of init-terminate generators of factory base.
	QList<generatorBase::parts::InitTerminateCodeGenerator *> initTerminateGenerators() override;

	/// Generator name is used as a prefix to a path to templates in resources.
	const QString mGeneratorName;

	/// Storage and generator for human-readable goto labels. Used in label ganerator and goto generator created by
	/// this factory.
	GotoLabelManager &mGotoLabelManager;

	/// Generator part that tracks LED usage and initializes it if needed.
	QScopedPointer<LedPart> mLedPart;

	/// Generator part that tracks Tof usage and initializes it if needed.
	QScopedPointer<TofPart> mTofPart;

	/// Generator part that tracks magnet usage and initializes it if needed.
	QScopedPointer<MagnetPart> mMagnetPart;

	/// Generator part that tracks random number generator usage and initializes it if needed.
	QScopedPointer<RandomGeneratorPart> mRandomGeneratorPart;
};

}
}
