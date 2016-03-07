/* Copyright 2016 Dmitry Mordvinov
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

#include "metaMetaModel/elementType.h"

#include <qrgraph/queries.h>

#include "metaMetaModel/metamodel.h"
#include "metaMetaModel/nodeElementType.h"
#include "metaMetaModel/edgeElementType.h"
#include "metaMetaModel/patternType.h"

using namespace qReal;

ElementType::ElementType(Metamodel &metamodel)
	: qrgraph::Node(metamodel)
{
}

Metamodel &ElementType::metamodel() const
{
	return *static_cast<Metamodel *>(&graph());
}

const NodeElementType &ElementType::toNode() const
{
	Q_ASSERT(type() == Type::node);
	return *dynamic_cast<const NodeElementType *>(this);
}

const EdgeElementType &ElementType::toEdge() const
{
	Q_ASSERT(type() == Type::edge);
	return *dynamic_cast<const EdgeElementType *>(this);
}

const PatternType &ElementType::toPattern() const
{
	Q_ASSERT(type() == Type::pattern);
	return *dynamic_cast<const PatternType *>(this);
}

bool ElementType::isParent(const ElementType &parent) const
{
	return qrgraph::Queries::isReachableInTree(*this, parent, generalizationLinkType);
}

IdList ElementType::containedTypes() const
{
	QSet<Id> result;
	qrgraph::Queries::treeLift(*this, [&result](const Node &parent) {
		const QList<const Node *> nodes = qrgraph::Queries::immediateFollowers(parent, containmentLinkType);
		for (const qrgraph::Node *node : nodes) {
			if (const ElementType *type = dynamic_cast<const ElementType *>(node)) {
				result.insert(Id(type->metamodel().id(), type->diagram(), type->name()));
			}
		}

		return false;
	}, generalizationLinkType);
	return result.toList();
}

void ElementType::updateRendererContent(const QString &shape)
{
	Q_UNUSED(shape)
}
