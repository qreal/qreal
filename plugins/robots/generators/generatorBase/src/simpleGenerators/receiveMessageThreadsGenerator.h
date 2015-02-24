#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace generatorBase {
namespace simple {

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
