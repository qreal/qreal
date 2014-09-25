#include "receiveMessageGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

ReceiveMessageGenerator::ReceiveMessageGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "messages/receiveMessage.t"
			, { Binding::createDirect("@@VARIABLE@@", "Variable") }
			, parent)
{
}
