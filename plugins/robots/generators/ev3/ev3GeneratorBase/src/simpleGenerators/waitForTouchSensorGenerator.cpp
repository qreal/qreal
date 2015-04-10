#include "waitForTouchSensorGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "src/ev3GeneratorCustomizer.h"

using namespace ev3::simple;
using namespace generatorBase::simple;

WaitForTouchSensorGenerator::WaitForTouchSensorGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "sensors/touch.t", QList<Binding *>()
			<< Binding::createStaticConverting("@@Block@@", "Block"
					, static_cast<Ev3GeneratorFactory *>(customizer.factory())->goToBlockNumberConverter())
			<< Binding::createConverting("@@Port@@", "Port", customizer.factory()->portNameConverter())
			, parent)
{
}
