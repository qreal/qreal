#pragma once

#include <generatorBase/simpleGenerators/abstractSimpleGenerator.h>

namespace generatorBase {
namespace simple {

/// Generates code that joins a number of threads into one, `main` thread.
class JoinGenerator : public AbstractSimpleGenerator
{
public:
	JoinGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, const QStringList &joinedThreads
			, const QString &mainThreadId
			, QObject *parent = 0);

	QString generate() override;

private:
	QStringList mJoinedThreads;
	QString mMainThreadId;
};

}
}
