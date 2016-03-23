/* Copyright 2016 CyberTech Labs Ltd, Dmitry Mordvinov
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

#include <QtXml/QDomDocument>

#include <qrgraph/queries.h>

#include "metaMetaModel/metamodel.h"
#include "metaMetaModel/nodeElementType.h"
#include "metaMetaModel/edgeElementType.h"
#include "metaMetaModel/patternType.h"

using namespace qReal;

ElementType::ElementType(Metamodel &metamodel)
	: qrgraph::Node(metamodel)
	, mSdf(new QDomDocument)
	, mIsHidden(false)
{
}

ElementType::~ElementType()
{
}

Metamodel &ElementType::metamodel() const
{
	return *static_cast<Metamodel *>(&graph());
}

NodeElementType &ElementType::toNode()
{
	Q_ASSERT(type() == Type::node);
	return *dynamic_cast<NodeElementType *>(this);
}

const NodeElementType &ElementType::toNode() const
{
	Q_ASSERT(type() == Type::node);
	return *dynamic_cast<const NodeElementType *>(this);
}

EdgeElementType &ElementType::toEdge()
{
	Q_ASSERT(type() == Type::edge);
	return *dynamic_cast<EdgeElementType *>(this);
}

const EdgeElementType &ElementType::toEdge() const
{
	Q_ASSERT(type() == Type::edge);
	return *dynamic_cast<const EdgeElementType *>(this);
}

PatternType &ElementType::toPattern()
{
	Q_ASSERT(type() == Type::pattern);
	return *dynamic_cast<PatternType *>(this);
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

QString ElementType::name() const
{
	return mName;
}

void ElementType::setName(const QString &name)
{
	mName = name;
}

QString ElementType::friendlyName() const
{
	return mFriendlyName;
}

void ElementType::setFriendlyName(const QString &friendlyName)
{
	mFriendlyName = friendlyName;
}

QString ElementType::description() const
{
	return mDescription;
}

void ElementType::setDescription(const QString &description)
{
	mDescription = description;
}

QString ElementType::diagram() const
{
	return mDiagram;
}

void ElementType::setDiagram(const QString &diagramName)
{
	mDiagram = diagramName;
}

QDomElement ElementType::sdf() const
{
	return mSdf.isNull() ? QDomElement() : mSdf->documentElement();
}

void ElementType::loadSdf(const QDomElement &picture)
{
	if (mSdf->isNull()) {
		mSdf->appendChild(mSdf->importNode(picture, true));
		return;
	}

	QDomElement currentPicture = mSdf->documentElement();
	for (QDomElement child = picture.firstChildElement(); !child.isNull(); child = child.nextSiblingElement()) {
		currentPicture.insertBefore(mSdf->importNode(child, true), currentPicture.firstChildElement());
	}
}

const QList<LabelProperties> &ElementType::labels() const
{
	return mLabels;
}

void ElementType::addLabel(const LabelProperties &label)
{
	mLabels << label;
}

const QStringList &ElementType::propertyNames() const
{
	return mPropertyNames;
}

const QStringList &ElementType::referenceProperties() const
{
	return mReferenceProperties;
}

QString ElementType::propertyType(const QString &name) const
{
	return mPropertyTypes[name];
}

QString ElementType::propertyDefaultValue(const QString &property) const
{
	return mPropertyDefaultValues[property];
}

QString ElementType::propertyDescription(const QString &property) const
{
	return mPropertyDescriptions[property];
}

QString ElementType::propertyDisplayedName(const QString &property) const
{
	return mPropertyDisplayedNames[property];
}

void ElementType::addProperty(const QString &name, const QString &type, const QString &defaultValue
		, const QString &displayedName, const QString &description, bool isReference)
{
	if (!mPropertyNames.contains(name)) {
		mPropertyNames << name;
	}

	if (isReference && !mReferenceProperties.contains(name)) {
		mReferenceProperties << name;
	}

	mPropertyTypes[name] = type;
	mPropertyDefaultValues[name] = defaultValue;
	mPropertyDisplayedNames[name] = displayedName;
	mPropertyDescriptions[name] = description;
}

bool ElementType::isHidden() const
{
	return mIsHidden;
}

void ElementType::setHidden(bool isHidden)
{
	mIsHidden = isHidden;
}
