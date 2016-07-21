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

#include "ev3RbfGeneratorFactory.h"

#include "simpleGenerators/prependedCodeGenerator.h"

using namespace ev3::rbf;

Ev3RbfGeneratorFactory::Ev3RbfGeneratorFactory(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName)
	: Ev3GeneratorFactory(repo, errorReporter, robotModelManager, luaProcessor, generatorName)
{
}

generatorBase::simple::AbstractSimpleGenerator *Ev3RbfGeneratorFactory::labelGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer)
{
	return new simple::PrependedCodeGenerator(mRepo, customizer, id, this);
}

QStringList Ev3RbfGeneratorFactory::additionalCode(const qReal::Id &id) const
{
	return mAdditionalCode[id].isEmpty() ? QStringList() : (mAdditionalCode[id] + QStringList() << QString());
}

void Ev3RbfGeneratorFactory::addAdditionalCode(const qReal::Id &id, const QStringList &code)
{
	mAdditionalCode[id] += code;
}
