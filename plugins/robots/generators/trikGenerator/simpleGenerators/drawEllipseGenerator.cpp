#include "drawEllipseGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DrawEllipseGenerator::DrawEllipseGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/drawEllipse.t", QList<Binding *>()
			<< Binding::createConverting("@@XCoordinateEllipse@@", "XCoordinateEllipse", customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@YCoordinateEllipse@@", "YCoordinateEllipse", customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@WidthEllipse@@", "WidthEllipse", customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@HeightEllipse@@", "HeightEllipse", customizer.factory()->intPropertyConverter())
			, parent)
{
}
