#include "systemGenerator.h"

#include <generatorCustomizer.h>

using namespace qReal::robots::generators::simple;

SystemGenerator::SystemGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "system.t"
			, QList<Binding *>()
					<< Binding::createDirect("@@COMMAND@@", "Command")
			, parent)
{
}
