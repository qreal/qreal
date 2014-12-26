#include "sendMessageGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

SendMessageGenerator::SendMessageGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "messages/sendMessage.t"
			, { Binding::createDirect("@@MESSAGE@@", "Message")
					, Binding::createDirect("@@HULL_NUMBER@@", "HullNumber") }
			, parent)
{
}
