#pragma once

#include "../base/generatorCustomizer.h"
#include "nxtOsekGeneratorFactory.h"

namespace qReal {
namespace robots {
namespace generators {
namespace nxtOsek {

class NxtOsekGeneratorCustomizer : public GeneratorCustomizer
{
public:
	explicit NxtOsekGeneratorCustomizer(LogicalModelAssistInterface const &model);

	virtual GeneratorFactoryBase *factory();

private:
	NxtOsekGeneratorFactory *mFactory;
};

}
}
}
}
