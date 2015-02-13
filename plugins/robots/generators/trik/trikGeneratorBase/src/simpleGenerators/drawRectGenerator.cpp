#include "drawRectGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DrawRectGenerator::DrawRectGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/drawRect.t"
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
}
