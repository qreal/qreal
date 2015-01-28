#include "detectorToVariableGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DetectorToVariableGenerator::DetectorToVariableGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "function.t"
			, { Binding::createStaticConverting("@@BODY@@"
					, repo.property(id, "Variable").toString() + "= lineSensorX"
					, customizer.factory()->functionBlockConverter(id, "Variable"))
			}, parent)
{
}
