#pragma once

#include "generatorBase/simpleGenerators/abstractSimpleGenerator.h"

namespace generatorBase {
namespace simple {

/// Generates code that instantiates new threads.
class ForkCallGenerator : public AbstractSimpleGenerator
{
public:
	ForkCallGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, const qReal::IdList &threads
		, QObject *parent = 0);

	QString generate() override;

private:
	const qReal::IdList mThreads;
};

}
}
