#include "drawLineGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace ev3::simple;
using namespace generatorBase::simple;

DrawLineGenerator::DrawLineGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
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
}
