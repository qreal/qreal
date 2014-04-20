#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for conditional construnctions
class IfElementGenerator : public BindingGenerator
{
public:
	IfElementGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, qReal::Id const &id
		, bool elseIsEmpty
		, bool needInverting
		, QObject *parent = 0);
};

}
}
