#include "enginesGenerator.h"
#include "src/converters/portNameConverter.h"

#include <generatorBase/generatorCustomizer.h>

using namespace ev3::simple;
using namespace generatorBase::simple;

EnginesGenerator::EnginesGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QString const &engineType
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, engineType.contains("EnginesBackward")
					? "engines/backward.t"
					: "engines/forward.t"
			, {
			Binding::createConverting("@@Port@@", "Ports"
					, customizer.factory()->portNameConverter())
			, Binding::createConverting("@@Power@@", "Power"
					, customizer.factory()->intPropertyConverter(id, "Power"))
			}
			, parent)
{
	//mCustomizer.factory()->engines()->registerUsageOnPorts(repo.property(id, "Ports").toString());
}
