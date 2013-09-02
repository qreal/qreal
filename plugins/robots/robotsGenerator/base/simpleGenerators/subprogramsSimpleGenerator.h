#pragma once

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class SubprogramsSimpleGenerator : public BindingGenerator
{
public:
	SubprogramsSimpleGenerator(LogicalModelAssistInterface const &model
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);

	static QString identifier(qReal::Id const &id);
};

}
}
}
}
