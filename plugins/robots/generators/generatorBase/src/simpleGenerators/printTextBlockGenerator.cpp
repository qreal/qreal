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

#include "printTextBlockGenerator.h"
#include "generatorBase/generatorCustomizer.h"

#include <qrutils/stringUtils.h>

using namespace generatorBase::simple;
using namespace qReal;

PrintTextBlockGenerator::PrintTextBlockGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/printText.t", {
			Binding::createConverting("@@X@@", "XCoordinateText"
					, customizer.factory()->intPropertyConverter(id, "XCoordinateText"))
			, Binding::createConverting("@@Y@@", "YCoordinateText"
					, customizer.factory()->intPropertyConverter(id, "YCoordinateText"))
			, (repo.property(id, "Evaluate").toBool()
					? Binding::createConverting("@@TEXT@@", "PrintText"
							, customizer.factory()->stringPropertyConverter(id, "PrintText"))
					: Binding::createStaticConverting("@@TEXT@@"
							, utils::StringUtils::wrap(repo.stringProperty(id, "PrintText"))
							, customizer.factory()->stringPropertyConverter(id, "PrintText")))
			}, parent)
{
	// Calling virtual readTemplate() before base class constructor will cause segfault.
	addBinding(Binding::createStatic("@@REDRAW@@", repo.property(id, "Redraw").toBool()
			? readTemplate("drawing/redraw.t") : QString()));
}
