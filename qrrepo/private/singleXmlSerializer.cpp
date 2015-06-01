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

#include "singleXmlSerializer.h"

#include <qrutils/outFile.h>

#include "classes/logicalObject.h"
#include "classes/graphicalObject.h"
#include "valuesSerializer.h"

using namespace qrRepo::details;
using namespace qReal;

void SingleXmlSerializer::exportToXml(const QString &targetFile, QHash<qReal::Id, Object*> const &objects)
{
	Q_ASSERT_X(!targetFile.isEmpty(), "XmlSerializer::exportTo(...)", "target filename is empty");

	QDomDocument doc;
	QDomElement root = doc.createElement("project");
	doc.appendChild(root);

	foreach (const Id &id, objects[Id::rootId()]->children()) {

		// skip logical elements of diagrams
		if (objects[id]->isLogicalObject()) {
			continue;
		}

		exportDiagram(id, doc, root, objects);
	}

	utils::OutFile out(targetFile);
	doc.save(out(), 4);
}

void SingleXmlSerializer::exportDiagram(const Id &diagramId, QDomDocument &doc, QDomElement &root
		, QHash<qReal::Id, Object*> const &objects)
{
	QDomElement diagram = doc.createElement("diagram");

	const GraphicalObject * const graphicalObject = dynamic_cast<const GraphicalObject *>(objects[diagramId]);
	if (graphicalObject) {
		diagram.setAttribute("logical_id", graphicalObject->logicalId().toString());
	}

	diagram.setAttribute("graphical_id", diagramId.toString());
	diagram.setAttribute("name", objects[diagramId]->properties()["name"].toString());
	exportProperties(diagramId, doc, diagram, objects);

	QDomElement elements = doc.createElement("elements");

	foreach (const Id &id, objects[diagramId]->children()) {
		exportElement(id, doc, elements, objects);
	}

	diagram.appendChild(elements);

	root.appendChild(diagram);
}

void SingleXmlSerializer::exportElement(const Id &id, QDomDocument &doc, QDomElement &root
		, QHash<qReal::Id, Object*> const &objects)
{
	QDomElement element = doc.createElement("element");
	element.setAttribute("name", objects[id]->properties()["name"].toString());
	element.setAttribute("graphical_id", id.toString());

	const GraphicalObject * const graphicalObject = dynamic_cast<const GraphicalObject *>(objects[id]);
	if (graphicalObject) {
		element.setAttribute("logical_id", graphicalObject->logicalId().toString());
	}

	exportProperties(id, doc, element, objects);
	exportChildren(id, doc, element, objects);

	root.appendChild(element);
}

void SingleXmlSerializer::exportChildren(const Id &id, QDomDocument &doc, QDomElement &root
		, QHash<qReal::Id, Object*> const &objects)
{
	Object *object = objects[id];
	int size = object->children().size();
	if (size == 0) {
		return;
	}

	QDomElement children = doc.createElement("children");
	children.setAttribute("count", size);

	foreach (const Id &id, object->children()) {
		exportElement(id, doc, children, objects);
	}

	root.appendChild(children);
}

void SingleXmlSerializer::exportProperties(const Id&id, QDomDocument &doc, QDomElement &root
		, QHash<Id, Object *> const &objects)
{
	QDomElement props = doc.createElement("properties");

	const GraphicalObject * const graphicalObject = dynamic_cast<const GraphicalObject *>(objects[id]);
	const LogicalObject * const logicalObject
			= dynamic_cast<const LogicalObject *>(objects[graphicalObject->logicalId()]);

	QMap<QString, QVariant> properties;

	QMapIterator<QString, QVariant> i = logicalObject->propertiesIterator();
	while (i.hasNext()) {
		i.next();
		properties[i.key()] = i.value();

	}

	i = graphicalObject->propertiesIterator();
	while (i.hasNext()) {
		i.next();
		properties[i.key()] = i.value();
	}

	foreach (const QString &key, properties.keys()) {
		QDomElement prop = doc.createElement("property");

		QString typeName = properties[key].typeName();
		QVariant value = properties[key];
		if (typeName == "qReal::IdList" && (value.value<IdList>().size() != 0)) {
			QDomElement list = ValuesSerializer::serializeIdList("list", value.value<IdList>(), doc);
			prop.appendChild(list);
		} else if (typeName == "qReal::Id"){
			prop.setAttribute("value", value.value<Id>().toString());
		} else if (value.toString().isEmpty()) {
			continue;
		} else {
			prop.setAttribute("value", properties[key].toString());
		}

		prop.setAttribute("name", key);

		props.appendChild(prop);
	}

	root.appendChild(props);
}
