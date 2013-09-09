#pragma once

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

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
