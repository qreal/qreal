#pragma once

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class FinalNodeGenerator : public BindingGenerator
{
public:
	FinalNodeGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, bool inMainDiagram
			, QObject *parent = 0);
};

}
}
}
}
