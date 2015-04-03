#include "drawCircleGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace ev3::simple;
using namespace generatorBase::simple;

DrawCircleGenerator::DrawCircleGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, (repo.stringProperty(id, "Filled") == "true") ? "drawing/drawFilledCircle.t" : "drawing/drawCircle.t"
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
}
