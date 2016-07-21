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

#include "zoneNode.h"

namespace generatorBase {
namespace semantics {

/// A base for all semantic nodes except zones
class NonZoneNode : public SemanticNode
{
public:
	explicit NonZoneNode(const qReal::Id &idBinded, QObject *parent = 0);

	ZoneNode *parentZone() const;

	void appendSibling(SemanticNode *node);
	void appendSiblings(QLinkedList<SemanticNode *> const &nodes);
	void insertSiblingAfterThis(SemanticNode *node);
};

}
}
