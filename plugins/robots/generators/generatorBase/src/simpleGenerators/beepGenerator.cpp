#include "beepGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

BeepGenerator::BeepGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "beep.t"
			, {Binding::createConverting("@@VOLUME@@", "Volume", customizer.factory()->intPropertyConverter(id))}
			, parent)
{
}
