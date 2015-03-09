#include "playToneGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

PlayToneGenerator::PlayToneGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "playTone.t"
			, QList<Binding *>()
					<< Binding::createDirect("@@FILENAME@@", "FileName")
			, parent)
{
}
