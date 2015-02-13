#include "finalNodeGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

FinalNodeGenerator::FinalNodeGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, bool inMainDiagram
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, inMainDiagram ? "finalNodeMain.t" : "finalNodeSubprogram.t"
			, QList<Binding *>(), parent)
{
}
