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

#include "waitForTouchSensorGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "src/ev3GeneratorCustomizer.h"

using namespace ev3::simple;
using namespace generatorBase::simple;

WaitForTouchSensorGenerator::WaitForTouchSensorGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "sensors/touch.t", QList<Binding *>()
			<< Binding::createStaticConverting("@@Block@@", "Block"
					, static_cast<Ev3GeneratorFactory *>(customizer.factory())->goToBlockNumberConverter())
			<< Binding::createConverting("@@Port@@", "Port", customizer.factory()->portNameConverter())
			, parent)
{
}