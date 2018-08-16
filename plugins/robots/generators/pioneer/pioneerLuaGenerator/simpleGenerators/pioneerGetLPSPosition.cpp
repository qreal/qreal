/* Copyright 2018 CyberTech Labs Ltd.
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

#include "pioneerGetLPSPosition.h"

#include <generatorBase/generatorCustomizer.h>

using namespace pioneer::lua;
using namespace generatorBase::simple;

PioneerGetLPSPosition::PioneerGetLPSPosition(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "quadcopterCommands/getLPSPosition.t"
		, {
			Binding::createConverting(
					"@@X@@"
					, "X"
					, customizer.factory()->stringPropertyConverter(id, "X"))
			, Binding::createConverting(
					"@@Y@@"
					, "Y"
					, customizer.factory()->stringPropertyConverter(id, "Y"))
			, Binding::createConverting(
					"@@Z@@"
					, "Z"
					, customizer.factory()->stringPropertyConverter(id, "Z"))
			}
		, parent)
{
}
