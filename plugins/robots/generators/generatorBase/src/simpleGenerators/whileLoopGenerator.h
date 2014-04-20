#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for loops in 'while-do' and 'do-while' forms
class WhileLoopGenerator : public BindingGenerator
{
public:
	WhileLoopGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, bool doWhileForm
			, bool needInverting
			, QObject *parent = 0);
};

}
}
