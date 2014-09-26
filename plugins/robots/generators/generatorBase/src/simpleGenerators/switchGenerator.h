#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for the first case of the 'Switch' block.
class SwitchGenerator : public BindingGenerator
{
public:
	SwitchGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QString const &part
			, QStringList const &values
			, QObject *parent = 0);
};

}
}
