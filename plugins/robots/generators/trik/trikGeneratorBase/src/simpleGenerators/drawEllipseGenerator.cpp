#include "drawEllipseGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DrawEllipseGenerator::DrawEllipseGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/drawEllipse.t"
			, {
					Binding::createConverting("@@XCoordinateEllipse@@", "XCoordinateEllipse"
							, customizer.factory()->intPropertyConverter(id, "XCoordinateEllipse"))
					, Binding::createConverting("@@YCoordinateEllipse@@", "YCoordinateEllipse"
							, customizer.factory()->intPropertyConverter(id, "YCoordinateEllipse"))
					, Binding::createConverting("@@WidthEllipse@@", "WidthEllipse"
							, customizer.factory()->intPropertyConverter(id, "WidthEllipse"))
					, Binding::createConverting("@@HeightEllipse@@", "HeightEllipse"
							, customizer.factory()->intPropertyConverter(id, "HeightEllipse"))
			}
			, parent)
{
}
