#include "sendMessageGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

SendMessageGenerator::SendMessageGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "messages/sendMessage.t"
			, { Binding::createDirect("@@MESSAGE@@", "Message")
					, Binding::createDirect("@@HULL_NUMBER@@", "HullNumber") }
			, parent)
{
}
