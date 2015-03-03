#include "waitPadPressGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace qReal;
using namespace generatorBase;
using namespace generatorBase::simple;

WaitPadPressGenerator::WaitPadPressGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/padPress.t"
			, { Binding::createConverting("@@PAD@@", "Pad", customizer.factory()->stringPropertyConverter(id, "Pad")) }
			, parent)
{
}
