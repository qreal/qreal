#include "finalNodeGenerator.h"

using namespace qReal::robots::generators::simple;

FinalNodeGenerator::FinalNodeGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, bool inMainDiagram
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, inMainDiagram ? "finalNodeMain.t" : "finalNodeSubprogram.t"
			, QList<Binding *>(), parent)
{
}
