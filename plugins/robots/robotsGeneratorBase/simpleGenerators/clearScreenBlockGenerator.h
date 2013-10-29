#pragma once

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'ClearScreen' block
class ClearScreenBlockGenerator : public BindingGenerator
{
public:
	ClearScreenBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);
};

}
}
}
}
