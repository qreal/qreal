#include "forkCallGenerator.h"

#include <qrutils/nameNormalizer.h>

using namespace generatorBase::simple;

ForkCallGenerator::ForkCallGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, qReal::Id const &id
		, qReal::IdList const &threads
		, QObject *parent)
	: AbstractSimpleGenerator(repo, customizer, id, parent)
	, mThreads(threads)
{
}

QString ForkCallGenerator::generate()
{
	QString result;
	QString const callPattern = readTemplate("threads/call.t");
	for (qReal::Id const &thread : mThreads) {
		QString const threadName = utils::NameNormalizer::normalizeStrongly(thread.id(), false);
		result += QString(callPattern).replace("@@NAME@@", threadName);
	}

	return result;
}
