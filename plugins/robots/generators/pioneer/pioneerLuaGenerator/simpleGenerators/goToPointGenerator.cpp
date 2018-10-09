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

#include "goToPointGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace pioneer::lua;
using namespace generatorBase::simple;

GoToPointGenerator::GoToPointGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "quadcopterCommands/goToLocalPoint.t"
		, {
			Binding::createConverting(
					"@@X@@"
					, "X"
					, customizer.factory()->intPropertyConverter(id, "X"))
			, Binding::createConverting(
					"@@Y@@"
					, "Y"
					, customizer.factory()->intPropertyConverter(id, "Y"))
			, Binding::createConverting(
					"@@Z@@"
					, "Z"
					, customizer.factory()->intPropertyConverter(id, "Z"))
			}
		, parent)
{
	const QString timeValue = mRepo.property(mId, "Time").toString();
	if (timeValue.isEmpty()) {
		addBinding(Binding::createStatic("@@VAR_ARG_SEPARATOR@@", QString()));
		addBinding(Binding::createStatic("@@Time@@", QString()));
	} else {
		Binding::ConverterInterface *intConverter = customizer.factory()->intPropertyConverter(id, "Time");
		addBinding(Binding::createStatic("@@VAR_ARG_SEPARATOR@@", readTemplate("luaPrinting/argumentsSeparator.t")));
		addBinding(Binding::createConverting("@@Time@@", "Time", intConverter));
	}
}
