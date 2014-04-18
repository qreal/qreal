#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for loops in 'while-do' and 'do-while' forms
class WhileLoopGenerator : public BindingGenerator
{
public:
	WhileLoopGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, bool doWhileForm
			, bool needInverting
			, QObject *parent = 0);
};

}
}
}
}
