#pragma once

#include <simpleGenerators/bindingGenerator.h>

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for various waitings for button press.
class WaitForButtonGenerator : public BindingGenerator
{
public:
	WaitForButtonGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QString const &buttonTemplate
			, QObject *parent);
};

}
}
}
}
