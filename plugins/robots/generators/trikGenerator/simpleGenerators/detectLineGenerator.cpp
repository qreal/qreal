#include "detectLineGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;

DetectLineGenerator::DetectLineGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "detectLine.t"
			, {}
			, parent)
{
}
