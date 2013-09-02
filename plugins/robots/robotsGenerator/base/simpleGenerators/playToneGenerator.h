#pragma once

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class PlayToneGenerator : public BindingGenerator
{
public:
	PlayToneGenerator(LogicalModelAssistInterface const &model
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);
};

}
}
}
}
