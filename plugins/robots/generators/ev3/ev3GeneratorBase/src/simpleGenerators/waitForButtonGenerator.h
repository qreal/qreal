#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace ev3 {
namespace simple {

/// Generator for 'WaitForButton' block
class WaitForButtonGenerator : public generatorBase::simple::BindingGenerator
{
public:
	WaitForButtonGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QString const &buttonTemplate
			, QObject *parent);
};

}
}
