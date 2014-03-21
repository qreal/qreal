#pragma once

#include <simpleGenerators/bindingGenerator.h>

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for "line detector reading to variable" block.
class LineDetectorToVariableGenerator : public BindingGenerator
{
public:
	LineDetectorToVariableGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent);
};

}
}
}
}
