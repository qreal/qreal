#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for conditional construnctions
class IfElementGenerator : public BindingGenerator
{
public:
	IfElementGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, bool elseIsEmpty
		, bool needInverting
		, QObject *parent = 0);
};

}
}
}
}
