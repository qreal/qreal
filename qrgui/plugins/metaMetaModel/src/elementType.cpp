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

const QString ElementType::editor() const
{
	return metamodel().id();
}

const Id ElementType::typeId() const
{
	return Id(editor(), diagram(), name());
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
				result.insert(type->typeId());
			}
		}

		return false;
	}, generalizationLinkType);

	return result.toList();
}

QList<const Explosion *> ElementType::explosions() const
{
	// Explosions set may be inherited from parent type. Children explosions override parent ones, so
	// if two explosions with the same target will be met then the one of the subclass will be prefered.
	QMap<const ElementType *, const Explosion *> result;
	qrgraph::Queries::treeLift(*this, [&result](const Node &parent) {
		for (const qrgraph::Edge *edge : parent.outgoingEdges(explosionLinkType)) {
			const Explosion *explosion = dynamic_cast<const Explosion *>(edge);
			if (explosion && !result.contains(&explosion->target())) {
				result[&explosion->target()] = explosion;
			}
		}

		return false;
	}, generalizationLinkType);

	return result.values();
}

void ElementType::updateRendererContent(const QString &shape)
{
	Q_UNUSED(shape)
}
