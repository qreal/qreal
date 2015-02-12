#include "waitGamepadDisconnectGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace generatorBase::simple;
using namespace qReal;
using namespace trik::simple;

WaitGamepadDisconnectGenerator::WaitGamepadDisconnectGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/gamepadDisconnect.t", {}, parent)
{
}
