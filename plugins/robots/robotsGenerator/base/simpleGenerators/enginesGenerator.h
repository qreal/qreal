#pragma once

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class EnginesGenerator : public BindingGenerator
{
public:
	EnginesGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QString const &engineType
			, QObject *parent);
};

}
}
}
}
