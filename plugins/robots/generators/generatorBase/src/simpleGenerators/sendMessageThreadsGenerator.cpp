#include "sendMessageThreadsGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;

SendMessageThreadsGenerator::SendMessageThreadsGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "threads/sendMessage.t"
			, { Binding::createConverting("@@MESSAGE@@"
					, "Message"
					, customizer.factory()->functionBlockConverter(id, "Message"))
				, Binding::createDirect("@@THREAD_ID@@", "Thread") }
			, parent)
{
}
