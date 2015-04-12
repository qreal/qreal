#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace generatorBase {
namespace simple {

/// Generates receiving a message sent to this thread by another one.
class ReceiveMessageThreadsGenerator : public BindingGenerator
{
public:
	ReceiveMessageThreadsGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
