#include "playToneGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

PlayToneGenerator::PlayToneGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "playTone.t", QList<Binding *>()
			<< Binding::createConverting("@@FREQUENCY@@", "Frequency"
					, customizer.factory()->intPropertyConverter(id, "Frequency"))
			<< Binding::createConverting("@@DURATION@@", "Duration"
					, customizer.factory()->intPropertyConverter(id, "Duration"))
			<< Binding::createConverting("@@VOLUME@@", "Volume"
					, customizer.factory()->intPropertyConverter(id, "Volume"))
			, parent)
{
}
