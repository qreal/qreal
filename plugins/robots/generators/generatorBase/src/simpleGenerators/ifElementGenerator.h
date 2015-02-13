#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for conditional construnctions
class IfElementGenerator : public BindingGenerator
{
public:
	IfElementGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, bool elseIsEmpty
		, bool needInverting
		, QObject *parent = 0);
};

}
}
