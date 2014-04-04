#include "lineDetectorToVariableGenerator.h"

#include <generatorCustomizer.h>

using namespace qReal::robots::generators::simple;

LineDetectorToVariableGenerator::LineDetectorToVariableGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "lineDetectorToVariable.t"
			, {Binding::createDirect("@@VAR@@", "Variable")}
			, parent)
{
}
