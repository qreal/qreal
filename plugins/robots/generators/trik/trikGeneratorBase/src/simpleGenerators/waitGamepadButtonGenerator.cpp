#include "waitGamepadButtonGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace qReal;
using namespace generatorBase::simple;

WaitGamepadButtonGenerator::WaitGamepadButtonGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/gamepadButton.t"
			, {Binding::createConverting(
					"@@BUTTON@@", "Button", customizer.factory()->stringPropertyConverter(id, "Button"))
					}
			, parent)
{
}
