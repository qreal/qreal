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

#include "drawRectGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace ev3::simple;
using namespace generatorBase::simple;

DrawRectGenerator::DrawRectGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, (repo.stringProperty(id, "Filled") == "true")
					? "drawing/drawFilledRect.t"
					: "drawing/drawRect.t"
			, {
				Binding::createConverting("@@XCoordinateRect@@", "XCoordinateRect"
						, customizer.factory()->intPropertyConverter(id, "XCoordinateRect"))
				, Binding::createConverting("@@YCoordinateRect@@", "YCoordinateRect"
						, customizer.factory()->intPropertyConverter(id, "YCoordinateRect"))
				, Binding::createConverting("@@WidthRect@@", "WidthRect"
						, customizer.factory()->intPropertyConverter(id, "WidthRect"))
				, Binding::createConverting("@@HeightRect@@", "HeightRect"
						, customizer.factory()->intPropertyConverter(id, "HeightRect"))
			}
			, parent)
{
	addBinding(Binding::createStatic("@@REDRAW@@", repo.property(id, "Redraw").toBool()
			? readTemplate("drawing/redraw.t") : QString()));
}
