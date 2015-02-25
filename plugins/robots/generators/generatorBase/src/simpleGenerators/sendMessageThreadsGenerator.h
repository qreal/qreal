#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace generatorBase {
namespace simple {

/// Generates sending a message to another thread.
class SendMessageThreadsGenerator : public BindingGenerator
{
public:
	SendMessageThreadsGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
