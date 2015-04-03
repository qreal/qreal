#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace ev3 {
namespace simple {

class DrawCircleGenerator : public generatorBase::simple::BindingGenerator
{
public:
	DrawCircleGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent);
};

}
}
