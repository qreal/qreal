#include "printTextBlockGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

PrintTextBlockGenerator::PrintTextBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/printText.t", QList<Binding *>()
			<< Binding::createConverting("@@X@@", "XCoordinateText"
					, customizer.factory()->intPropertyConverter(id, "XCoordinateText"))
			<< Binding::createConverting("@@Y@@", "YCoordinateText"
					, customizer.factory()->intPropertyConverter(id, "YCoordinateText"))
			<< Binding::createConverting("@@TEXT@@", "PrintText", customizer.factory()->stringPropertyConverter())
			, parent)
{
}
