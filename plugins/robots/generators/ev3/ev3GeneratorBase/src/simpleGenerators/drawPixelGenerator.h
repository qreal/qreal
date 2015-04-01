#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace ev3 {
namespace simple {

class DrawPixelGenerator : public generatorBase::simple::BindingGenerator
{
public:
	DrawPixelGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent);
};

}
}
