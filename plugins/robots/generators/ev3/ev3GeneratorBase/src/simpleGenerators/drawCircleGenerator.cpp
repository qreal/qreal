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

#include "drawCircleGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace ev3::simple;
using namespace generatorBase::simple;

DrawCircleGenerator::DrawCircleGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, (repo.stringProperty(id, "Filled") == "true")
					? "drawing/drawFilledCircle.t"
					: "drawing/drawCircle.t"
			, {
				Binding::createConverting("@@XCoordinateCircle@@", "XCoordinateCircle"
						, customizer.factory()->intPropertyConverter(id, "XCoordinateCircle"))
				, Binding::createConverting("@@YCoordinateCircle@@", "YCoordinateCircle"
						, customizer.factory()->intPropertyConverter(id, "YCoordinateCircle"))
				, Binding::createConverting("@@CircleRadius@@", "CircleRadius"
						, customizer.factory()->intPropertyConverter(id, "CircleRadius"))
			}
			, parent)
{
	addBinding(Binding::createStatic("@@REDRAW@@", repo.property(id, "Redraw").toBool()
			? readTemplate("drawing/redraw.t") : QString()));
}
