#include "forkCallGenerator.h"

#include <qrutils/nameNormalizer.h>

using namespace generatorBase::simple;

ForkCallGenerator::ForkCallGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const qReal::Id &id
		, const qReal::IdList &threads
		, QObject *parent)
	: AbstractSimpleGenerator(repo, customizer, id, parent)
	, mThreads(threads)
{
}

QString ForkCallGenerator::generate()
{
	QString result;
	const QString callPattern = readTemplate("threads/call.t");
	for (const qReal::Id &thread : mThreads) {
		const QString threadName = utils::NameNormalizer::normalizeStrongly(thread.id(), false);
		result += QString(callPattern).replace("@@NAME@@", threadName);
	}

	return result;
}
