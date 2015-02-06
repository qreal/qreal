#include "initCameraGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;

InitCameraGenerator::InitCameraGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "videosensors/init" + repo.property(id, "Mode").toString() + "Sensor.t"
			, {}
			, parent)
{
}
