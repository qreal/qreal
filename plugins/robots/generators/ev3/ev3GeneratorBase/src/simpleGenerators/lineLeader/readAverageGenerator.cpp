/* Copyright 2017 CyberTech Labs Ltd.
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

#include "readAverageGenerator.h"

#include <generatorBase/generatorCustomizer.h>

#include "ev3GeneratorBase/ev3GeneratorFactory.h"

using namespace ev3::simple::lineLeader;
using namespace generatorBase::simple;
using namespace qReal;

ReadAverageGenerator::ReadAverageGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "sensors/lineLeader/readAverage.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port"
					, static_cast<Ev3GeneratorFactory *>(customizer.factory())->portNameConverter())
			, parent)
{
	Binding::ConverterInterface *nameNormalizer = customizer.factory()->nameNormalizerConverter();
	const QString variable = nameNormalizer->convert(mRepo.property(mId, "Variable").toString());
	addBinding(Binding::createStatic("@@VARIABLE@@", variable));
	customizer.factory()->functionBlockConverter(id, "Variable")->convert(QString("%1 = 0").arg(variable));
}
