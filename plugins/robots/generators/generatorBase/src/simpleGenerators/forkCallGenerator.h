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
		, const QMap<qReal::Id, QString> &threads
		, QObject *parent = 0);

	QString generate() override;

private:
	const QMap<qReal::Id, QString> mThreads;
};

}
}
