#include "joinGenerator.h"

using namespace generatorBase::simple;

JoinGenerator::JoinGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, const QStringList &joinedThreads
		, const QString &mainThreadId
		, QObject *parent)
	: AbstractSimpleGenerator(repo, customizer, id, parent)
	, mJoinedThreads(joinedThreads)
	, mMainThreadId(mainThreadId)
{
}

QString JoinGenerator::generate()
{
	if (mJoinedThreads.contains(mMainThreadId)) {
		return readTemplate(mMainThreadId == "main" ? "finalNodeMain.t" : "finalNodeSubprogram.t");
	}

	QString result;
	const QString callPattern = readTemplate("threads/join.t");

	QSet<QString> joinedThreads = mJoinedThreads.toSet();
	for (const QString &threadId : joinedThreads) {
		result += QString(callPattern).replace("@@THREAD_ID@@", threadId);
	}

	return result;
}

