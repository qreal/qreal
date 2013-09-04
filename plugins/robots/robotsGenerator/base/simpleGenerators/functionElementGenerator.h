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
		, bool generateToInit
		, QObject *parent = 0);

private:
	bool const mGenerateToInit;
};

}
}
}
}
