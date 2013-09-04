#include "waitForSoundBlockGenerator.h"

using namespace qReal::robots::generators::simple;

WaitForSoundBlockGenerator::WaitForSoundBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "waitSound.t", QList<Binding *>(), parent)
{
}
