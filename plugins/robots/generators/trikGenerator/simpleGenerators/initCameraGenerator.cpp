#include "initCameraGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;

InitCameraGenerator::InitCameraGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "videosensors/init" + repo.property(id, "Mode").toString() + "Sensor.t"
			, {}
			, parent)
{
}
