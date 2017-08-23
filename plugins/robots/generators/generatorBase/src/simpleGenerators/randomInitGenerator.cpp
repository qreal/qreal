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

#include "randomInitGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;

RandomInitGenerator::RandomInitGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "variables/randomInitialization.t"
			, { Binding::createConverting("@@VARIABLE@@"
					, "Variable"
					, customizer.factory()->functionBlockConverter(id, "Variable"))
				, Binding::createStaticConverting("@@FROM@@"
					, repo.property(id, "LowerBound").toString()
					, customizer.factory()->functionBlockConverter(id, "LowerBound"))
				, Binding::createStaticConverting("@@TO@@"
					, repo.property(id, "UpperBound").toString()
					, customizer.factory()->functionBlockConverter(id, "UpperBound")) }
			, parent)
{
}
