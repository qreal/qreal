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

#include "ledGenerator.h"

#include <generatorBase/generatorCustomizer.h>

#include "ev3GeneratorBase/ev3GeneratorFactory.h"

using namespace ev3::simple;
using namespace generatorBase::simple;

LedGenerator::LedGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "led/led.t"
			, { Binding::createConverting("@@COLOR@@", "Color"
					, static_cast<Ev3GeneratorFactory *>(customizer.factory())->ledColorConverter()) }
			, parent)
{
}
