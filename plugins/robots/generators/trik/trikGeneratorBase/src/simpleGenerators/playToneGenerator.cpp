#include "playToneGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

PlayToneGenerator::PlayToneGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "playTone.t"
			, QList<Binding *>()
					<< Binding::createDirect("@@FILENAME@@", "FileName")
			, parent)
{
}
