#pragma once

#include "generatorBase/simpleGenerators/abstractSimpleGenerator.h"

namespace generatorBase {
namespace simple {

/// Generates code that instantiates new threads.
class ForkCallGenerator : public AbstractSimpleGenerator
{
public:
	ForkCallGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, qReal::Id const &id
		, qReal::IdList const &threads
		, QObject *parent = 0);

	QString generate() override;

private:
	qReal::IdList const mThreads;
};

}
}
