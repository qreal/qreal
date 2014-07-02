#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for 'NullifyEncoder' block for TRIK
class TrikNullificationEncoderGenerator: public generatorBase::simple::BindingGenerator
{
public:
	TrikNullificationEncoderGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);
};

}
}
