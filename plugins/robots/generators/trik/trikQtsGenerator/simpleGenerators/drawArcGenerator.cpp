#include "drawArcGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DrawArcGenerator::DrawArcGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/drawArc.t"
			, {
					Binding::createConverting("@@XCoordinateArc@@", "XCoordinateArc"
							, customizer.factory()->intPropertyConverter(id))
					, Binding::createConverting("@@YCoordinateArc@@", "YCoordinateArc"
							, customizer.factory()->intPropertyConverter(id))
					, Binding::createConverting("@@WidthArc@@", "WidthArc"
							, customizer.factory()->intPropertyConverter(id))
					, Binding::createConverting("@@HeightArc@@", "HeightArc"
							, customizer.factory()->intPropertyConverter(id))
					, Binding::createConverting("@@StartAngle@@", "StartAngle"
							, customizer.factory()->intPropertyConverter(id))
					, Binding::createConverting("@@SpanAngle@@", "SpanAngle"
							, customizer.factory()->intPropertyConverter(id))
			}
			, parent)
{
}

