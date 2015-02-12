#include "detectGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;

DetectGenerator::DetectGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "videosensors/detect" + repo.property(id, "Mode").toString() + ".t"
			, {}
			, parent)
{
}
