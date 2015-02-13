#include "beepGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

BeepGenerator::BeepGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "beep.t"
			, {Binding::createConverting("@@VOLUME@@", "Volume"
					, customizer.factory()->intPropertyConverter(id, "Volume"))}
			, parent)
{
}
