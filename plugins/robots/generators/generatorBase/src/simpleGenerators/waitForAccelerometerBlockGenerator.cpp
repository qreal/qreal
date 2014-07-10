#include "waitForAccelerometerBlockGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

WaitForAccelerometerBlockGenerator::WaitForAccelerometerBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/accelerometer.t", QList<Binding *>(), parent)
{
}
