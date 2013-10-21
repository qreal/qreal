#include "beepGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

BeepGenerator::BeepGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "beep.t", QList<Binding *>()
			<< Binding::createConverting("@@VOLUME@@", "Volume", customizer.factory()->intPropertyConverter())
			, parent)
{
}
