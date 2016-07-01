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

#include "metaMetaModel/metamodel.h"

using namespace qReal;

Metamodel::Metamodel(const QString &id)
	: mId(id)
{
}

QString Metamodel::id() const
{
	return mId;
}

QString Metamodel::version() const
{
	return mVersion;
}

void Metamodel::setVersion(const QString &version)
{
	mVersion = version;
}

const QStringList &Metamodel::diagrams() const
{
	return mDiagrams;
}

void Metamodel::setDiagrams(const QStringList &diagrams)
{
	mDiagrams = diagrams;
}

void Metamodel::addDiagram(const QString &diagramName)
{
	mDiagrams << diagramName;
}

QList<ElementType *> Metamodel::elements(const QString &diagram) const
{
	return mElements[diagram].values();
}

ElementType &Metamodel::elementType(const Id &id) const
{
	return elementType(id.diagram(), id.element());
}

ElementType &Metamodel::elementType(const QString &diagram, const QString &element) const
{
	Q_ASSERT_X(mElements.contains(diagram) && mElements[diagram].contains(element)
			, Q_FUNC_INFO, "No such entity in metamodel!");
	ElementType * const result = mElements[diagram][element];
	Q_ASSERT_X(result, Q_FUNC_INFO, "No such entity in metamodel!");
	return *result;
}

void Metamodel::addElement(ElementType &element)
{
	addNode(element);
}

QStringList Metamodel::enumNames() const
{
	return mEnumValues.keys();
}

QList<QPair<QString, QString>> Metamodel::enumValues(const QString &name) const
{
	return mEnumValues.values(name);
}

void Metamodel::addEnum(const QString &name, const QList<QPair<QString, QString> > &values)
{
	for (auto &&value : values) {
		mEnumValues.insert(name, value);
	}
}

bool Metamodel::isEnumEditable(const QString &name) const
{
	return mEnumsEditability[name];
}

void Metamodel::setEnumEditable(const QString &name, bool editable)
{
	mEnumsEditability[name] = editable;
}

QString Metamodel::friendlyName() const
{
	return mFriendlyName;
}

void Metamodel::setFriendlyName(const QString &friendlyName)
{
	mFriendlyName = friendlyName;
}

QString Metamodel::diagramFriendlyName(const QString &diagram) const
{
	return mDiagramFriendlyNames[diagram];
}

void Metamodel::setDiagramFriendlyName(const QString &diagram, const QString &friendlyName)
{
	mDiagramFriendlyNames[diagram] = friendlyName;
}

ElementType *Metamodel::diagramNode(const QString &diagram) const
{
	return mDiagramNodes[diagram].isEmpty() ? nullptr : &elementType(diagram, mDiagramNodes[diagram]);
}

void Metamodel::setDiagramNode(const QString &diagram, const QString &elementName)
{
	mDiagramNodes[diagram] = elementName;
}

QStringList Metamodel::diagramPaletteGroups(const QString &diagram) const
{
	return mPaletteGroups[diagram];
}

void Metamodel::appendDiagramPaletteGroup(const QString &diagram, const QString &group)
{
	mPaletteGroups[diagram] << group;
}

QStringList Metamodel::diagramPaletteGroupList(const QString &diagram, const QString &group) const
{
	return mPaletteGroupContents[diagram][group];
}

void Metamodel::addElementToDiagramPaletteGroup(const QString &diagram, const QString &group, const QString &element)
{
	mPaletteGroupContents[diagram][group] << element;
}

QString Metamodel::diagramPaletteGroupDescription(const QString &diagram, const QString &group) const
{
	return mPaletteGroupDescriptions[diagram][group];
}

void Metamodel::setDiagramPaletteGroupDescription(const QString &diagram, const QString &group
		, const QString &description)
{
	mPaletteGroupDescriptions[diagram][group] = description;
}

bool Metamodel::shallPaletteBeSorted(const QString &diagram) const
{
	return mPaletteSorting[diagram];
}

void Metamodel::setPaletteSorted(const QString &diagram, bool sorted)
{
	mPaletteSorting[diagram] = sorted;
}

void Metamodel::addNode(qrgraph::Node &entity)
{
	ElementType * const type = dynamic_cast<ElementType *>(&entity);
	Q_ASSERT_X(type, Q_FUNC_INFO, "Attempt to add non-metamodel enitity!");
	Q_ASSERT_X(&type->metamodel() == this, Q_FUNC_INFO, "Attempt to add entity from other metamodel!");

	const QString diagram = type->diagram();
	const QString element = type->name();
	Q_ASSERT_X(!mElements[diagram][element], Q_FUNC_INFO, "Duplicate enitity in metamodel");

	mElements[diagram][element] = type;
	Multigraph::addNode(entity);
}

void Metamodel::addExplosion(ElementType &source, ElementType &target, bool isReusable, bool requiresImmediateLinkage)
{
	Explosion *explosion = new Explosion(*this, isReusable, requiresImmediateLinkage);
	addEdge(*explosion);
	explosion->connectBegin(source);
	explosion->connectEnd(target);
}
