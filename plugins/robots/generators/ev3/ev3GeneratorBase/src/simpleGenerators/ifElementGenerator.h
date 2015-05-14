#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace ev3 {
namespace simple {

/// Generator for conditional construnctions
class IfElementGenerator : public generatorBase::simple::BindingGenerator
{
public:
	IfElementGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, bool elseIsEmpty
		, bool needInverting
		, QObject *parent = 0);
};

}
}

