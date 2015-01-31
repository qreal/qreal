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
		, QMap<qReal::Id, QString> const &threads
		, QObject *parent = 0);

	QString generate() override;

private:
	QMap<qReal::Id, QString> const mThreads;
};

}
}
