#include "receiveMessageThreadsGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;

ReceiveMessageThreadsGenerator::ReceiveMessageThreadsGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "threads/receiveMessage.t"
			, { Binding::createConverting("@@VARIABLE@@"
					, "Variable"
					, customizer.factory()->functionBlockConverter(id, "Variable")) }
			, parent)
{
}
