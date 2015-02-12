#include "printTextBlockGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

PrintTextBlockGenerator::PrintTextBlockGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/printText.t", QList<Binding *>()
			<< Binding::createConverting("@@X@@", "XCoordinateText"
					, customizer.factory()->intPropertyConverter(id, "XCoordinateText"))
			<< Binding::createConverting("@@Y@@", "YCoordinateText"
					, customizer.factory()->intPropertyConverter(id, "YCoordinateText"))
			<< Binding::createConverting("@@TEXT@@", "PrintText"
					, customizer.factory()->stringPropertyConverter(id, "PrintText"))
			, parent)
{
}
