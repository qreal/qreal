#include "playToneGenerator.h"

#include <generatorCustomizer.h>

using namespace qReal::robots::generators::simple;

PlayToneGenerator::PlayToneGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "playTone.t"
			, QList<Binding *>()
					<< Binding::createDirect("@@FILENAME@@", "FileName")
			, parent)
{
}
