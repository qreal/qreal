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

#include "pioneerLedGenerator.h"

#include <qrkernel/exception/exception.h>
#include <generatorBase/generatorCustomizer.h>
#include <qrutils/stringUtils.h>

#include "pioneerLuaGeneratorFactory.h"
#include "parts/ledPart.h"

using namespace pioneer::lua;
using namespace generatorBase::simple;

PioneerLedGenerator::PioneerLedGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "led.t"
		, {
			Binding::createConverting(
					"@@NUMBER@@"
					, "Number"
					, customizer.factory()->intPropertyConverter(id, "Number"))
			, Binding::createConverting(
					"@@RED@@"
					, "Red"
					, customizer.factory()->floatPropertyConverter(id, "Red"))
			, Binding::createConverting(
					"@@GREEN@@"
					, "Green"
					, customizer.factory()->floatPropertyConverter(id, "Green"))
			, Binding::createConverting(
					"@@BLUE@@"
					, "Blue"
					, customizer.factory()->floatPropertyConverter(id, "Blue"))
			}
		, parent)
{
	auto factory = dynamic_cast<PioneerLuaGeneratorFactory *>(mCustomizer.factory());
	if (factory) {
		factory->ledPart().registerUsage();
	} else {
		throw qReal::Exception("Pioneer PioneerLedGenerator will work only with PioneerLuaGeneratorFactory");
	}
}
