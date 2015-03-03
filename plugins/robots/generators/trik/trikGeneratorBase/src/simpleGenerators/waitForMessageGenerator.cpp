#include "waitForMessageGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

WaitForMessageGenerator::WaitForMessageGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "messages/waitForMessage.t"
			, { Binding::createDirect("@@VARIABLE@@", "Variable") }
			, parent)
{
}
