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

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtCore/QString>
#include <QtGui/QIcon>

#include <qrgraph/multigraph.h>

#include "metaMetaModel/sdfRendererInterface.h"
#include "metaMetaModel/elementType.h"

namespace qReal {

/// Describes the metamodel of some visual langages.
/// The metamodel is a set of types of some entities associated each with other with some relations
/// (for example relation of generalization, relation of explosion, relation of containment, etc).
/// Metamodel thus is ordered multigraph where nodes are types of entities and edges are those relations.
/// This fact is noted with inheriting Metamodel from qrgraph::Multigraph.
class Metamodel : public qrgraph::Multigraph
{
public:
	struct ExplosionData
	{
	public:
		ExplosionData(const QString &targetDiagram, const QString &targetElement
				, bool isReusable, bool requiresImmediateLinkage)
			: targetDiagram(targetDiagram)
			, targetElement(targetElement)
			, isReusable(isReusable)
			, requiresImmediateLinkage(requiresImmediateLinkage)
		{
		}

		QString targetDiagram;
		QString targetElement;
		bool isReusable;
		bool requiresImmediateLinkage;
	};

	virtual void initPlugin() = 0;
	virtual QString id() const = 0;
	virtual QString version() const = 0;

	virtual QStringList diagrams() const = 0;
	virtual QStringList elements(const QString &diagram) const = 0;

	virtual QStringList getPropertiesWithDefaultValues(const QString &element) const = 0;
	virtual QStringList getTypesContainedBy(const QString &element) const = 0;
	virtual QList<ExplosionData> explosions(const QString &diagram, const QString &element) const = 0;
	virtual QList<QPair<QPair<QString, QString>, QPair<bool, QString> > >
			getPossibleEdges(const QString &element) const = 0;

	virtual int isNodeOrEdge(const QString &element) const = 0;

	virtual ElementType *getGraphicalObject(const QString &diagram, const QString &element) const = 0;
	virtual QStringList getPropertyNames(const QString &diagram, const QString &element) const = 0;
	virtual QStringList getPortTypes(const QString &diagram, const QString &element) const = 0;
	virtual QStringList getReferenceProperties(const QString &diagram, const QString &element) const = 0;
	virtual QString getPropertyType(const QString &element, const QString &name) const = 0;
	virtual QString getPropertyDefaultValue(const QString &element, const QString &property) const = 0;
	virtual QList<QPair<QString, QString>> getEnumValues(const QString &name) const = 0;
	virtual bool isEnumEditable(const QString &name) const = 0;
	virtual QString getGroupsXML() const = 0;
	virtual QList<QPair<QString, QString> > getParentsOf(const QString &diagram, const QString &element) const = 0;

	virtual QString editorName() const = 0;
	virtual QString diagramName(const QString &diagram) const = 0;
	virtual QString diagramNodeName(const QString &diagram) const = 0;
	virtual QString elementName(const QString &diagram, const QString &element) const = 0;
	virtual QString elementMouseGesture(const QString &diagram, const QString &element) const = 0;
	virtual QString elementDescription(const QString &diagram, const QString &element) const = 0;
	virtual QString propertyDescription(const QString &diagram, const QString &element
			, const QString &property) const = 0;
	virtual QString propertyDisplayedName(const QString &diagram, const QString &element
			, const QString &property) const = 0;

	virtual bool isParentOf(const QString &parentDiagram, const QString &parentElement
			, const QString &childDiagram, const QString &childElement) const = 0;

	virtual QStringList diagramPaletteGroups(const QString &diagram) const = 0;
	virtual QStringList diagramPaletteGroupList(const QString &diagram, const QString &group) const = 0;
	virtual QString diagramPaletteGroupDescription(const QString &diagram, const QString &group) const = 0;
	virtual bool shallPaletteBeSorted(const QString &diagram) const = 0;
};

}

Q_DECLARE_INTERFACE(qReal::Metamodel, "ru.spbsu.QReal.Metamodel/1.0")
