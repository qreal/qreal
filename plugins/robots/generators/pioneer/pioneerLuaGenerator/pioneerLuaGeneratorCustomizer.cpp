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

#include "pioneerLuaGeneratorCustomizer.h"

#include "pioneerLuaGeneratorFactory.h"

using namespace pioneer::lua;

PioneerLuaGeneratorCustomizer::PioneerLuaGeneratorCustomizer(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName
		, GotoLabelManager &gotoLabelManager
		, bool supportsSwitchUnstableToBreaks)
	: mFactory(
			new PioneerLuaGeneratorFactory(
					repo
					, errorReporter
					, robotModelManager
					, luaProcessor
					, generatorName
					, gotoLabelManager
			))
	, mSupportsSwitchUnstableToBreaks(supportsSwitchUnstableToBreaks)
{
}

generatorBase::GeneratorFactoryBase *PioneerLuaGeneratorCustomizer::factory()
{
	return mFactory.data();
}

bool PioneerLuaGeneratorCustomizer::supportsSwitchUnstableToBreaks() const
{
	return mSupportsSwitchUnstableToBreaks;
}
