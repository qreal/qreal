#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace nxt {
namespace simple {

/// Generator for 'ClearScreen' block in NXT kit.
class ClearScreenBlockGenerator : public generatorBase::simple::BindingGenerator
{
public:
	ClearScreenBlockGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);
};

}
}
