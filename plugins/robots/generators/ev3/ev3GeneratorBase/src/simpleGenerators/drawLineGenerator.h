#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace ev3 {
namespace simple {

class DrawLineGenerator : public generatorBase::simple::BindingGenerator
{
public:
	DrawLineGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent);
};

}
}

