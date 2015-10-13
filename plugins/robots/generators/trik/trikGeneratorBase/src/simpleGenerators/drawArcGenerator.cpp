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

#include "drawArcGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DrawArcGenerator::DrawArcGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/drawArc.t"
			, {
					Binding::createConverting("@@XCoordinateArc@@", "XCoordinateArc"
							, customizer.factory()->intPropertyConverter(id, "XCoordinateArc"))
					, Binding::createConverting("@@YCoordinateArc@@", "YCoordinateArc"
							, customizer.factory()->intPropertyConverter(id, "YCoordinateArc"))
					, Binding::createConverting("@@WidthArc@@", "WidthArc"
							, customizer.factory()->intPropertyConverter(id, "WidthArc"))
					, Binding::createConverting("@@HeightArc@@", "HeightArc"
							, customizer.factory()->intPropertyConverter(id, "HeightArc"))
					, Binding::createConverting("@@StartAngle@@", "StartAngle"
							, customizer.factory()->intPropertyConverter(id, "StartAngle"))
					, Binding::createConverting("@@SpanAngle@@", "SpanAngle"
							, customizer.factory()->intPropertyConverter(id, "SpanAngle"))
			}
			, parent)
{
	// Calling virtual readTemplate() before base class constructor will cause segfault.
	addBinding(Binding::createStatic("@@REDRAW@@", repo.property(id, "Redraw").toBool()
			? readTemplate("drawing/redraw.t") : QString()));
}
