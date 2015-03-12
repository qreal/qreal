#include "forkCallGenerator.h"

#include <qrutils/nameNormalizer.h>

using namespace generatorBase::simple;

ForkCallGenerator::ForkCallGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, const QMap<qReal::Id, QString> &threads
		, QObject *parent)
	: AbstractSimpleGenerator(repo, customizer, id, parent)
	, mThreads(threads)
{
}

QString ForkCallGenerator::generate()
{
	QString result;
	const QString callPattern = readTemplate("threads/call.t");
	for (const qReal::Id &thread : mThreads.keys()) {
		const QString threadName = utils::NameNormalizer::normalizeStrongly(thread.id(), false);
		const QString threadId = mThreads[thread];
		result += QString(callPattern).replace("@@THREAD_ID@@", threadId).replace("@@NAME@@", threadName);
	}

	return result;
}
