#pragma once

#include "abstractSimpleGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

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
