#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace ev3 {
namespace simple {

class DrawRectGenerator : public generatorBase::simple::BindingGenerator
{
public:
	DrawRectGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent);
};

}
}

