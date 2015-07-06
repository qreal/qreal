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

