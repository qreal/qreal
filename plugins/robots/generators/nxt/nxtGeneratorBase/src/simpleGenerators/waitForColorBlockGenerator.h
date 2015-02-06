#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace nxt {
namespace simple {

/// Generator for 'WaitForColor' block
class WaitForColorBlockGenerator : public generatorBase::simple::BindingGenerator
{
public:
	WaitForColorBlockGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
