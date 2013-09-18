#pragma once

#include "../base/generatorFactoryBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace trik {

class TrikGeneratorFactory : public GeneratorFactoryBase
{
public:
	TrikGeneratorFactory(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);

	virtual ~TrikGeneratorFactory();

	virtual QString pathToTemplates() const;
};

}
}
}
}
