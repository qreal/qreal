#include "detectLineGenerator.h"

#include <generatorCustomizer.h>

using namespace qReal::robots::generators::simple;

DetectLineGenerator::DetectLineGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "detectLine.t"
			, {}
			, parent)
{
}
