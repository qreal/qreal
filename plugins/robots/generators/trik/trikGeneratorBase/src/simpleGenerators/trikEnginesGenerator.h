#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for 'EnginesForward', 'EnginesBackward' and 'AngularServo' block
class TrikEnginesGenerator : public generatorBase::simple::BindingGenerator
{
public:
	TrikEnginesGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, const QString &engineType
			, QObject *parent);
};

}
}
