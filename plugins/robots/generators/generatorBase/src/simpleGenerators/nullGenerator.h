#pragma once

#include "generatorBase/simpleGenerators/abstractSimpleGenerator.h"

namespace generatorBase {
namespace simple {

/// This generator always returns an empty string
class NullGenerator : public AbstractSimpleGenerator
{
public:
	NullGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);

	virtual QString generate();
};

}
}
