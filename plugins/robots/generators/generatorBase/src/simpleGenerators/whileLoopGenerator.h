#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for loops in 'while-do' and 'do-while' forms
class WhileLoopGenerator : public BindingGenerator
{
public:
	WhileLoopGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, bool doWhileForm
			, bool needInverting
			, QObject *parent = 0);
};

}
}
