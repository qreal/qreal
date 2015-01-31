#include "forkCallGenerator.h"

#include <qrutils/nameNormalizer.h>

using namespace generatorBase::simple;

ForkCallGenerator::ForkCallGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, qReal::Id const &id
		, const QMap<qReal::Id, QString> &threads
		, QObject *parent)
	: AbstractSimpleGenerator(repo, customizer, id, parent)
	, mThreads(threads)
{
}

QString ForkCallGenerator::generate()
{
	QString result;
	QString const callPattern = readTemplate("threads/call.t");
	for (qReal::Id const &thread : mThreads.keys()) {
		QString const threadName = utils::NameNormalizer::normalizeStrongly(thread.id(), false);
		QString const threadId = mThreads[thread];
		result += QString(callPattern).replace("@@THREAD_ID@@", threadId).replace("@@NAME@@", threadName);
	}

	return result;
}
