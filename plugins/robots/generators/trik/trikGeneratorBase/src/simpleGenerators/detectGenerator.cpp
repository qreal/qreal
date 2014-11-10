#include "detectGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;

DetectGenerator::DetectGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "videosensors/detect" + repo.property(id, "Mode").toString() + ".t"
			, {}
			, parent)
{
}
