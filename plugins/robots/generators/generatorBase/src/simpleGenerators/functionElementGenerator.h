#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'Function' block
class FunctionElementGenerator : public BindingGenerator
{
public:
	FunctionElementGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent = 0);

	virtual QString generate();

private:
	bool const mGenerateToInit;
};

}
}
}
}
