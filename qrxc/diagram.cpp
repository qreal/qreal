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

#include "diagram.h"

#include "type.h"
#include "enumType.h"
#include "numericType.h"
#include "stringType.h"
#include "portType.h"
#include "nodeType.h"
#include "edgeType.h"
#include "editor.h"

#include <QtCore/QDebug>

Diagram::Diagram(const QString &name, const QString &nodeName, const QString &displayedName, Editor *editor)
	: mDiagramName(name)
	, mDiagramNodeName(nodeName)
	, mDiagramDisplayedName(displayedName)
	, mEditor(editor)
	, mShallPaletteBeSorted(true)
{
}

Diagram::~Diagram()
{
	qDeleteAll(mTypes);
}

bool Diagram::init(const QDomElement &diagramElement)
{
	for (QDomElement element = diagramElement.firstChildElement();
		!element.isNull();
		element = element.nextSiblingElement())
	{
		if (element.nodeName() == "graphicTypes") {
			if (!initGraphicTypes(element)) {
				return false;
			}
		} else if (element.nodeName() == "nonGraphicTypes") {
			if (!initNonGraphicTypes(element)) {
				return false;
			}
		} else if (element.nodeName() == "palette") {
			initPalette(element);
		} else {
			qDebug() << "ERROR: unknown tag" << element.nodeName();
		}
	}

	return true;
}

bool Diagram::initGraphicTypes(const QDomElement &graphicTypesElement)
{
	for (QDomElement element = graphicTypesElement.firstChildElement();
		!element.isNull();
		element = element.nextSiblingElement())
	{
		if (element.nodeName() == "node") {
			Type *nodeType = new NodeType(this);
			if (!nodeType->init(element, mDiagramName)) {
				delete nodeType;
				qDebug() << "Can't parse node";
				return false;
			}
			mTypes[nodeType->qualifiedName()] = nodeType;
		} else if (element.nodeName() == "edge") {
			Type *edgeType = new EdgeType(this);
			if (!edgeType->init(element, mDiagramName)) {
				delete edgeType;
				qDebug() << "Can't parse edge";
				return false;
			}
			mTypes[edgeType->qualifiedName()] = edgeType;
		} else if (element.nodeName() == "import") {
			ImportSpecification import = {
					element.attribute("name", "")
					, element.attribute("as", "")
					, element.attribute("displayedName", "")
			};
			mImports.append(import);
		}
		else
		{
			qDebug() << "ERROR: unknown graphic type" << element.nodeName();
			return false;
		}
	}
	return true;
}

bool Diagram::initNonGraphicTypes(const QDomElement &nonGraphicTypesElement)
{
	for (QDomElement element = nonGraphicTypesElement.firstChildElement();
		!element.isNull();
		element = element.nextSiblingElement())
	{
		if (element.nodeName() == "groups") {
			mGroupsXML = "";			QString xml;
			QTextStream stream(&xml);
			element.save(stream, 1);
			xml.replace("\"", "\\\"");
			xml.replace("\n", "\\n");
			mGroupsXML = xml;
		} else if (element.nodeName() == "enum") {
			Type *enumType = new EnumType();
			if (!enumType->init(element, mDiagramName)) {
				delete enumType;
				qDebug() << "Can't parse enum";
				return false;
			}
			mTypes[enumType->qualifiedName()] = enumType;
		} else if (element.nodeName() == "numeric") {
			Type *numericType = new NumericType();
			if (!numericType->init(element, mDiagramName)) {
				delete numericType;
				qDebug() << "Can't parse numeric type";
				return false;
			}
			mTypes[numericType->qualifiedName()] = numericType;
		} else if (element.nodeName() == "string") {
			Type *stringType = new StringType();
			if (!stringType->init(element, mDiagramName)) {
				delete stringType;
				qDebug() << "Can't parse string type";
				return false;
			}
			mTypes[stringType->qualifiedName()] = stringType;
		} else if (element.nodeName() == "port") {
			Type *portType = new PortType();
			if (!portType->init(element, mDiagramName)) {
				delete portType;
				qDebug() << "Can't parse port type";
				return false;
			}
			mTypes[portType->qualifiedName()] = portType;
		}
		else {
			qDebug() << "ERROR: unknown non graphic type" << element.nodeName();
			return false;
		}
	}
	return true;
}

void Diagram::initPalette(const QDomElement &paletteElement)
{
	mShallPaletteBeSorted = paletteElement.attribute("sorted", "true") == "true";

	for (QDomElement element = paletteElement.firstChildElement("group");
		!element.isNull();
		element = element.nextSiblingElement("group"))
	{
		QString name = element.attribute("name");
		QString description = element.attribute("description", "");
		mPaletteGroupsDescriptions[name] = description;

		QStringList groupElements;
		for (QDomElement groupElement = element.firstChildElement("element");
			!groupElement.isNull();
			groupElement = groupElement.nextSiblingElement("element"))
		{
			groupElements << groupElement.attribute("name");
		}

		mPaletteGroups << qMakePair(name, groupElements);
	}
}

bool Diagram::resolve()
{
	foreach (ImportSpecification import, mImports) {
		Type *importedType = mEditor->findType(import.name);
		if (importedType == nullptr) {
			qDebug() << "ERROR: imported type" << import.name << "not found, skipping";
			continue;
		}
		Type *copiedType = importedType->clone();
		copiedType->setName(import.as);
		copiedType->setDisplayedName(import.displayedName);
		copiedType->setDiagram(this);
		copiedType->setContext(mDiagramName);
		mTypes.insert(copiedType->qualifiedName(), copiedType);
	}

	foreach(Type *type, mTypes.values())
		if (!type->resolve()) {
			qDebug() << "ERROR: can't resolve type" << type->name();
			return false;
		}

	return true;
}

Editor* Diagram::editor() const
{
	return mEditor;
}

Type* Diagram::findType(QString name)
{
	if (mTypes.contains(name)) {
		return mTypes[name];
	} else {
		return mEditor->findType(name);
	}
}

QMap<QString, Type*> Diagram::types() const
{
	return mTypes;
}

QString Diagram::name() const
{
	return mDiagramName;
}

QString Diagram::nodeName() const
{
	return mDiagramNodeName;
}

QString Diagram::displayedName() const
{
	return mDiagramDisplayedName;
}

QList<QPair<QString, QStringList>> Diagram::paletteGroups() const
{
	return mPaletteGroups;
}

QMap<QString, QString> Diagram::paletteGroupsDescriptions() const
{
	return mPaletteGroupsDescriptions;
}

bool Diagram::shallPaletteBeSorted() const
{
	return mShallPaletteBeSorted;
}

QString Diagram::getGroupsXML() const
{
	return mGroupsXML;
}
