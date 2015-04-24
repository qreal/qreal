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

#include "generatorBase/semanticTree/rootNode.h"

#include "generatorBase/semanticTree/simpleNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

RootNode::RootNode(SemanticNode * const initialBlock, QObject *parent)
	: SemanticNode(Id(), parent)
	, mZone(new ZoneNode(this))
	, mInitialBlock(initialBlock->id())
{
	mZone->setParentNode(this);
	mZone->appendChild(initialBlock);
}

Id RootNode::initialBlock() const
{
	return mInitialBlock;
}

QString RootNode::toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const
{
	return mZone->toString(customizer, indent, indentString);
}

QLinkedList<SemanticNode *> RootNode::children() const
{
	return QLinkedList<SemanticNode *>() << mZone;
}
