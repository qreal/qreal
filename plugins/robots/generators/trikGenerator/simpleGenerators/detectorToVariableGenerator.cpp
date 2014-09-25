#include "detectorToVariableGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DetectorToVariableGenerator::DetectorToVariableGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "videosensors/" + repo.property(id, "Mode").toString() + "DetectorToVariable.t"
			, {Binding::createDirect("@@VAR@@", "Variable")}
			, parent)
{
}
