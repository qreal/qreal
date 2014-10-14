#include "waitForMessageGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

WaitForMessageGenerator::WaitForMessageGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "messages/waitForMessage.t"
			, { Binding::createDirect("@@VARIABLE@@", "Variable") }
			, parent)
{
}
