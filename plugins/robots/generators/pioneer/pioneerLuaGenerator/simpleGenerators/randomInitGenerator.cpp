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

#include "randomInitGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include <qrkernel/exception/exception.h>

#include "pioneerLuaGeneratorFactory.h"
#include "parts/randomGeneratorPart.h"

using namespace pioneer::lua;
using namespace generatorBase::simple;

RandomInitGenerator::RandomInitGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
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
	const QString separator = !repo.stringProperty(id, "LowerBound").isEmpty()
			&& !repo.stringProperty(id, "UpperBound").isEmpty()
					? readTemplate("luaPrinting/fieldInitializersSeparator.t") : "";
	addBinding(Binding::createStatic("@@SEP@@", separator));

	auto factory = dynamic_cast<PioneerLuaGeneratorFactory *>(mCustomizer.factory());
	if (factory) {
		factory->randomGeneratorPart().registerUsage();
	} else {
		throw qReal::Exception("Pioneer RandomInitGenerator will work only with PioneerLuaGeneratorFactory");
	}
}
