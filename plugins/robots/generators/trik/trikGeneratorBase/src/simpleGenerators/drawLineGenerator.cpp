#include "drawLineGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
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
}
