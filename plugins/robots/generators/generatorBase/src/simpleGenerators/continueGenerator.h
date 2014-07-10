#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'continue' construction
class ContinueGenerator : public BindingGenerator
{
public:
	ContinueGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);
};

}
}
