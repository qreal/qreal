#pragma once

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class IfElementGenerator : public BindingGenerator
{
public:
	IfElementGenerator(LogicalModelAssistInterface const &model
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
