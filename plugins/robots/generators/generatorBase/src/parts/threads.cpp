/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "generatorBase/parts/threads.h"

#include <qrutils/nameNormalizer.h>

using namespace generatorBase;
using namespace parts;

Threads::Threads(const QStringList &pathsToTemplates)
	: TemplateParametrizedEntity(pathsToTemplates)
{
}

void Threads::registerThread(const qReal::Id &id, const QString &threadId)
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

void Threads::threadProcessed(const qReal::Id &id, semantics::SemanticTree &tree)
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
	for (const semantics::SemanticTree *thread : threads()) {
		result << name(thread);
	}

	return result;
}

void Threads::addJoin(const qReal::Id &id, const QString &threadId)
{
	mJoins[id] << threadId;
}

QStringList Threads::joinedThreads(const qReal::Id &id) const
{
	return mJoins[id];
}

QString Threads::generateDeclarations() const
{
	const QStringList threads = threadNames();
	const QString forwardDeclaration = readTemplate("threads/forwardDeclaration.t");
	if (forwardDeclaration.isEmpty() || threads.isEmpty()) {
		return QString();
	}

	const QString declarationsHeader = readTemplate("threads/declarationsSectionHeader.t");
	QStringList declarations;
	for (const QString &thread : threads) {
		declarations << QString(forwardDeclaration).replace("@@NAME@@", thread);
	}

	return declarationsHeader + declarations.join("\n");
}

QString Threads::generateImplementations(const QString &indentString) const
{
	QList<semantics::SemanticTree *> const threads = this->threads();
	const QString implementation = readTemplate("threads/implementation.t");
	if (implementation.isEmpty() || threads.isEmpty()) {
		return QString();
	}

	const QString implementationsHeader = readTemplate("threads/implementationsSectionHeader.t");
	QStringList implementations;
	for (const semantics::SemanticTree *thread : threads) {
		const QString code = thread->toString(1, indentString);
		implementations << QString(implementation).replace("@@NAME@@", name(thread)).replace("@@BODY@@", code);
	}

	return implementationsHeader + implementations.join("\n");
}

QString Threads::name(const semantics::SemanticTree *tree) const
{
	return utils::NameNormalizer::normalizeStrongly(tree->initialBlock().id(), false);
}
