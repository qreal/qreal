#pragma once

#include "../base/generatorFactoryBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace nxtOsek {

class NxtOsekGeneratorFactory : public GeneratorFactoryBase
{
public:
	explicit NxtOsekGeneratorFactory(qrRepo::RepoApi const &repo);
	virtual ~NxtOsekGeneratorFactory();

	virtual QString pathToTemplates() const;
};

}
}
}
}
