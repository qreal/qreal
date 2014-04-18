#pragma once

#include "generatorBase/simpleGenerators/abstractSimpleGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// This generator always returns an empty string
class NullGenerator : public AbstractSimpleGenerator
{
public:
	NullGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);

	virtual QString generate();
};

}
}
}
}
