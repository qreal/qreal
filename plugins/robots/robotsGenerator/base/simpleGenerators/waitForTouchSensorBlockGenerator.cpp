#include "waitForTouchSensorBlockGenerator.h"

using namespace qReal::robots::generators::simple;

WaitForTouchSensorBlockGenerator::WaitForTouchSensorBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "waitTouch.t", QList<Binding *>(), parent)
{
}
