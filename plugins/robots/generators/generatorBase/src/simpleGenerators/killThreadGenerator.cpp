#include "killThreadGenerator.h"

using namespace generatorBase::simple;

KillThreadGenerator::KillThreadGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "threads/killThread.t"
			, { Binding::createDirect("@@THREAD_ID@@", "Thread") }
			, parent)
{
}
