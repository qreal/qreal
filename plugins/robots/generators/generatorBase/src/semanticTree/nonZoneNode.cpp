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

#include "generatorBase/semanticTree/nonZoneNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

NonZoneNode::NonZoneNode(const Id &idBinded, QObject *parent)
	: SemanticNode(idBinded, parent)
{
}

ZoneNode *NonZoneNode::parentZone() const
{
	return static_cast<ZoneNode *>(mParentNode);
}

void NonZoneNode::appendSibling(SemanticNode *node)
{
	parentZone()->appendChild(node);
}

void NonZoneNode::appendSiblings(QLinkedList<SemanticNode *> const &nodes)
{
	parentZone()->appendChildren(nodes);
}

void NonZoneNode::insertSiblingAfterThis(SemanticNode *node)
{
	parentZone()->insertAfrer(this, node);
}
