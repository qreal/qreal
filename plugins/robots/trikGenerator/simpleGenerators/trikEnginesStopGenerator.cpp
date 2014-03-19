#include "trikEnginesStopGenerator.h"

#include <generatorCustomizer.h>
#include "trikGeneratorFactory.h"

using namespace qReal::robots::generators::simple;

TrikEnginesStopGenerator::TrikEnginesStopGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/stop.t", QList<Binding *>()
			<< Binding::createMultiTarget("@@PORT@@", "Ports"
					, dynamic_cast<trik::TrikGeneratorFactory *>(customizer.factory())->
							enginesConverter(repo.property(id, "powerMotors").toBool()))
			, parent)
{
}
