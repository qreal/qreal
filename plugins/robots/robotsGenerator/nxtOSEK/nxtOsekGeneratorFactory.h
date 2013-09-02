#pragma once

#include "../base/generatorFactoryBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace nxtOsek {

class NxtOsekGeneratorFactory : public GeneratorFactoryBase
{
public:
	explicit NxtOsekGeneratorFactory(LogicalModelAssistInterface const &model);
	virtual ~NxtOsekGeneratorFactory();

	virtual QString pathToTemplates() const;
};

}
}
}
}
