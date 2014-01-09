#include "waitForAccelerometerBlockGenerator.h"

using namespace qReal::robots::generators::simple;

WaitForAccelerometerBlockGenerator::WaitForAccelerometerBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/accelerometer.t", QList<Binding *>(), parent)
{
}
