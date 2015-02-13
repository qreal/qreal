#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for the first case of the 'Switch' block.
class SwitchGenerator : public BindingGenerator
{
public:
	SwitchGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, const QString &part
			, const QStringList &values
			, QObject *parent = 0);
};

}
}
