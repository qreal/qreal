#pragma once

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class ForLoopGenerator : public BindingGenerator
{
public:
	ForLoopGenerator(LogicalModelAssistInterface const &model
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);
};

}
}
}
}
