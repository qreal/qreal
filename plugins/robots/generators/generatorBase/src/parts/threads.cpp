#include "generatorBase/parts/threads.h"

#include <qrutils/nameNormalizer.h>

using namespace generatorBase;
using namespace parts;

Threads::Threads(QString const &pathToTemplates)
	: TemplateParametrizedEntity(pathToTemplates)
{
}

void Threads::registerThread(qReal::Id const &id, const QString &threadId)
{
	if (!mUnprocessedThreads.contains(id) && !mProcessedThreads.keys().contains(id)) {
		mUnprocessedThreads.insert(id);
		mThreadIds[id] = threadId;
	}
}

QString Threads::threadId(const qReal::Id &id) const
{
	return mThreadIds[id];
}

void Threads::threadProcessed(qReal::Id const &id, semantics::SemanticTree &tree)
{
	if (!mUnprocessedThreads.contains(id)) {
		return;
	}

	mProcessedThreads[id] = &tree;
	mUnprocessedThreads.remove(id);
}

bool Threads::hasUnprocessedThreads() const
{
	return !mUnprocessedThreads.empty();
}

qReal::Id Threads::nextUnprocessedThread() const
{
	return *mUnprocessedThreads.begin();
}

QList<semantics::SemanticTree *> Threads::threads() const
{
	return mProcessedThreads.values();
}

QStringList Threads::threadNames() const
{
	QStringList result;
	for (semantics::SemanticTree const *thread : threads()) {
		result << name(thread);
	}

	return result;
}

QString Threads::generateDeclarations() const
{
	QStringList const threads = threadNames();
	QString const forwardDeclaration = readTemplate("threads/forwardDeclaration.t");
	if (forwardDeclaration.isEmpty() || threads.isEmpty()) {
		return QString();
	}

	QString const declarationsHeader = readTemplate("threads/declarationsSectionHeader.t");
	QStringList declarations;
	for (QString const &thread : threads) {
		declarations << QString(forwardDeclaration).replace("@@NAME@@", thread);
	}

	return declarationsHeader + declarations.join("\n");
}

QString Threads::generateImplementations(QString const &indentString) const
{
	QList<semantics::SemanticTree *> const threads = this->threads();
	QString const implementation = readTemplate("threads/implementation.t");
	if (implementation.isEmpty() || threads.isEmpty()) {
		return QString();
	}

	QString const implementationsHeader = readTemplate("threads/implementationsSectionHeader.t");
	QStringList implementations;
	for (semantics::SemanticTree const *thread : threads) {
		QString const code = thread->toString(1, indentString);
		implementations << QString(implementation).replace("@@NAME@@", name(thread)).replace("@@BODY@@", code);
	}

	return implementationsHeader + implementations.join("\n");
}

QString Threads::name(semantics::SemanticTree const *tree) const
{
	return utils::NameNormalizer::normalizeStrongly(tree->initialBlock().id(), false);
}
