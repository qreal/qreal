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

#pragma once

#include <generatorBase/semanticTree/semanticTree.h>
#include <generatorBase/templateParametrizedEntity.h>

#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace parts {

/// A storage for all discovered threads in current save.
class ROBOTS_GENERATOR_EXPORT Threads : public TemplateParametrizedEntity
{
public:
	explicit Threads(const QStringList &pathsToTemplates);

	/// Must be called each time when generator gets into the block with the
	/// fork semantics for every block that start new thread.
	/// @param id The initial node of the thread
	void registerThread(const qReal::Id &id, const QString &threadId);

	/// Returns the id of a thread which starts at the given node
	QString threadId(const qReal::Id &id) const;

	/// Must be called each time when semantic tree for some thread was built.
	void threadProcessed(const qReal::Id &id, semantics::SemanticTree &tree);

	/// Returns true if not every registered semantic trees of treads was built.
	bool hasUnprocessedThreads() const;

	/// Returns first unprocessed semantic tree in the threads queue.
	qReal::Id nextUnprocessedThread() const;

	/// Returns a list of semantic trees of threads on all diagrams of the model.
	QList<semantics::SemanticTree *> threads() const;

	/// Returns a list of thread names on all diagrams of the model.
	QStringList threadNames() const;

	/// Adds a thread to a join point.
	void addJoin(const qReal::Id &id, const QString &threadId);

	/// Returns a list of names of the threads that are being joined at the given join node.
	QStringList joinedThreads(const qReal::Id &id) const;

	/// Generates and returns the code of the section with threads forward declarations.
	QString generateDeclarations() const;

	/// Generates and returns the code of the section with threads code.
	QString generateImplementations(const QString &indentString) const;

private:
	QString name(const semantics::SemanticTree *tree) const;

	QSet<qReal::Id> mUnprocessedThreads;
	QMap<qReal::Id, semantics::SemanticTree *> mProcessedThreads;
	QMap<qReal::Id, QString> mThreadIds;
	QMap<qReal::Id, QStringList> mJoins;
};

}
}
