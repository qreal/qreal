#pragma once

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class ContinueGenerator : public BindingGenerator
{
public:
	ContinueGenerator(LogicalModelAssistInterface const &model
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);
};

}
}
}
}
