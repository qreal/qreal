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

#include "generatorBase/semanticTree/forkNode.h"

#include <qrutils/stringUtils.h>

using namespace generatorBase::semantics;

ForkNode::ForkNode(const qReal::Id &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
{
}

void ForkNode::appendThread(const qReal::Id &thread, const QString &threadId)
{
	mThreads[thread] = threadId;
}

QLinkedList<SemanticNode *> ForkNode::children() const
{
	return {};
}

QString ForkNode::toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const
{
	const QString code = customizer.factory()->forkCallGenerator(mId, customizer, mThreads)->generate();
	return utils::StringUtils::addIndent(code, indent, indentString);
}
