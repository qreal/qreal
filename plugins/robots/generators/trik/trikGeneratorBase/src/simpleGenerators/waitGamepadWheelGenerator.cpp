#include "waitGamepadWheelGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace generatorBase::simple;
using namespace qReal;
using namespace trik::simple;

int const sleepInterval = 10;

WaitGamepadWheelGenerator::WaitGamepadWheelGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/gamepadWheel.t"
			, { Binding::createConverting("@@ANGLE@@", "Angle", customizer.factory()->intPropertyConverter(id, "Angle"))
					, Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter())
					}
			, parent)
{
}
