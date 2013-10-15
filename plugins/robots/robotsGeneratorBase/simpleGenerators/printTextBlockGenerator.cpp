#include "printTextBlockGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

PrintTextBlockGenerator::PrintTextBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/printText.t", QList<Binding *>()
			<< Binding::createConverting("@@X@@", "XCoordinateText", customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@Y@@", "YCoordinateText", customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@TEXT@@", "PrintText", customizer.factory()->stringPropertyConverter())
			, parent)
{
}
