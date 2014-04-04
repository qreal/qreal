#include "initCameraGenerator.h"

#include <generatorCustomizer.h>

using namespace qReal::robots::generators::simple;

InitCameraGenerator::InitCameraGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "initCamera.t"
			, {}
			, parent)
{
}
