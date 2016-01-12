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

#include "drawLineGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace ev3::simple;
using namespace generatorBase::simple;

DrawLineGenerator::DrawLineGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/drawLine.t"
			, {
				Binding::createConverting("@@X1CoordinateLine@@", "X1CoordinateLine"
						, customizer.factory()->intPropertyConverter(id, "X1CoordinateLine"))
				, Binding::createConverting("@@Y1CoordinateLine@@", "Y1CoordinateLine"
						, customizer.factory()->intPropertyConverter(id, "Y1CoordinateLine"))
				, Binding::createConverting("@@X2CoordinateLine@@", "X2CoordinateLine"
						, customizer.factory()->intPropertyConverter(id, "X2CoordinateLine"))
				, Binding::createConverting("@@Y2CoordinateLine@@", "Y2CoordinateLine"
						, customizer.factory()->intPropertyConverter(id, "Y2CoordinateLine"))
			}
			, parent)
{
	addBinding(Binding::createStatic("@@REDRAW@@", repo.property(id, "Redraw").toBool()
			? readTemplate("drawing/redraw.t") : QString()));
}
