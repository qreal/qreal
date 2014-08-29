#include "generatorBase/parts/threads.h"

#include <qrutils/nameNormalizer.h>

using namespace generatorBase;
using namespace parts;

Threads::Threads(QString const &pathToTemplates)
	: TemplateParametrizedEntity(pathToTemplates)
{
}

void Threads::registerThread(qReal::Id const &id)
{
	if (!mUnprocessedThreads.contains(id) && !mProcessedThreads.keys().contains(id)) {
		mUnprocessedThreads.insert(id);
	}
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

QString Threads::generateCode() const
{
	return generateDeclarations() + generateImplementations();
}

QString Threads::generateDeclarations() const
{
	QList<semantics::SemanticTree *> const threads = this->threads();
	QString const forwardDeclaration = readTemplate("threads/forwardDeclaration.t");
	if (forwardDeclaration.isEmpty() || threads.isEmpty()) {
		return QString();
	}

	QString const declarationsHeader = readTemplate("threads/declarationsSectionHeader.t");
	QStringList declarations;
	for (semantics::SemanticTree const *thread : threads) {
		declarations << QString(forwardDeclaration).replace("@@NAME@@", name(thread));
	}

	return declarationsHeader + declarations.join("\n");
}

QString Threads::generateImplementations() const
{
	QList<semantics::SemanticTree *> const threads = this->threads();
	QString const implementation = readTemplate("threads/implementation.t");
	if (implementation.isEmpty() || threads.isEmpty()) {
		return QString();
	}

	QString const implementationsHeader = readTemplate("threads/implementationsSectionHeader.t");
	QStringList implementations;
	for (semantics::SemanticTree const *thread : threads) {
		QString const code = thread->toString(1);
		implementations << QString(implementation).replace("@@NAME@@", name(thread)).replace("@@BODY@@", code);
	}

	return implementationsHeader + implementations.join("\n");
}

QString Threads::name(semantics::SemanticTree const *tree) const
{
	return utils::NameNormalizer::normalizeStrongly(tree->initialBlock().id(), false);
}
