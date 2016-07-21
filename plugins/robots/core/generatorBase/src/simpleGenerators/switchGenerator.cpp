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

#include "switchGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;

SwitchGenerator::SwitchGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, const QString &part
		, const QStringList &values
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, QString("switch/%1.t").arg(part), {
			Binding::createConverting("@@EXPRESSION@@", "Expression"
					, customizer.factory()->floatPropertyConverter(id, "Expression"))
			, Binding::createConverting("@@CONDITION@@", "Expression"
					, customizer.factory()->switchConditionsMerger(values))
			}, parent)
{
}
