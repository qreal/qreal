#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'Function' block
class FunctionElementGenerator : public BindingGenerator
{
public:
	FunctionElementGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent = 0);

	QString generate() override;

private:
	const bool mGenerateToInit;
};

}
}
