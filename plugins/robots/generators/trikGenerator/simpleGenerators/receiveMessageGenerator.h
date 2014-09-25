#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for 'Send message' block.
class ReceiveMessageGenerator : public generatorBase::simple::BindingGenerator
{
public:
	ReceiveMessageGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent);
};

}
}
