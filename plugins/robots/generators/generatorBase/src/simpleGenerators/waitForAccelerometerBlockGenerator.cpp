#include "waitForAccelerometerBlockGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

WaitForAccelerometerBlockGenerator::WaitForAccelerometerBlockGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/accelerometer.t", QList<Binding *>(), parent)
{
}
