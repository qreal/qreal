#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace nxt {
namespace simple {

/// Generator for 'WaitForColor' block
class WaitForColorBlockGenerator : public generatorBase::simple::BindingGenerator
{
public:
	WaitForColorBlockGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);
};

}
}
