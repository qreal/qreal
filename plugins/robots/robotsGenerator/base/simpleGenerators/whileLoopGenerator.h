#pragma once

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class WhileLoopGenerator : public BindingGenerator
{
public:
	WhileLoopGenerator(LogicalModelAssistInterface const &model
			, GeneratorCustomizer &customizer
			, Id const &id
			, bool doWhileForm
			, bool needInverting
			, QObject *parent = 0);
};

}
}
}
}
