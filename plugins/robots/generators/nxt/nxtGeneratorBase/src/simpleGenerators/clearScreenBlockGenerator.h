#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace nxt {
namespace simple {

/// Generator for 'ClearScreen' block in NXT kit.
class ClearScreenBlockGenerator : public generatorBase::simple::BindingGenerator
{
public:
	ClearScreenBlockGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
