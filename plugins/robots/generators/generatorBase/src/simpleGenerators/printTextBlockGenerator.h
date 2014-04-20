#pragma once

#include "drawBlockGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'PrintText' block
class PrintTextBlockGenerator : public BindingGenerator
{
public:
	PrintTextBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);
};

}
}
