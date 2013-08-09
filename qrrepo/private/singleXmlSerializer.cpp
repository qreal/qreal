#include "singleXmlSerializer.h"

#include "../../qrutils/outFile.h"
#include "classes/logicalObject.h"
#include "classes/graphicalObject.h"
#include "valuesSerializer.h"

using namespace qrRepo::details;
using namespace qReal;

void SingleXmlSerializer::exportToXml(QString const &targetFile, QHash<qReal::Id, Object*> const &objects)
{
	Q_ASSERT_X(!targetFile.isEmpty(), "XmlSerializer::exportTo(...)", "target filename is empty");

	QDomDocument doc;
	QDomElement root = doc.createElement("project");
	doc.appendChild(root);

	foreach (Id const &id, objects[Id::rootId()]->children()) {

		// skip logical elements of diagrams
		if (objects[id]->isLogicalObject()) {
			continue;
		}

		exportDiagram(id, doc, root, objects);
	}

	utils::OutFile out(targetFile);
	doc.save(out(), 4);
}

void SingleXmlSerializer::exportDiagram(Id const &diagramId, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects)
{
	QDomElement diagram = doc.createElement("diagram");

	GraphicalObject const * const graphicalObject = dynamic_cast<GraphicalObject const *>(objects[diagramId]);
	if (graphicalObject) {
		diagram.setAttribute("logical_id", graphicalObject->logicalId().toString());
	}

	diagram.setAttribute("graphical_id", diagramId.toString());
	diagram.setAttribute("name", objects[diagramId]->properties()["name"].toString());
	exportProperties(diagramId, doc, diagram, objects);

	QDomElement elements = doc.createElement("elements");

	foreach (Id const &id, objects[diagramId]->children()) {
		exportElement(id, doc, elements, objects);
	}

	diagram.appendChild(elements);

	root.appendChild(diagram);
}

void SingleXmlSerializer::exportElement(Id const &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects)
{
	QDomElement element = doc.createElement("element");
	element.setAttribute("name", objects[id]->properties()["name"].toString());
	element.setAttribute("graphical_id", id.toString());

	GraphicalObject const * const graphicalObject = dynamic_cast<GraphicalObject const *>(objects[id]);
	if (graphicalObject) {
		element.setAttribute("logical_id", graphicalObject->logicalId().toString());
	}

	exportProperties(id, doc, element, objects);
	exportChildren(id, doc, element, objects);

	root.appendChild(element);
}

void SingleXmlSerializer::exportChildren(Id const &id, QDomDocument &doc, QDomElement &root, QHash<qReal::Id, Object*> const &objects)
{
	Object *object = objects[id];
	int size = object->children().size();
	if (size == 0) {
		return;
	}

	QDomElement children = doc.createElement("children");
	children.setAttribute("count", size);

	foreach (Id const &id, object->children()) {
		exportElement(id, doc, children, objects);
	}

	root.appendChild(children);
}

void SingleXmlSerializer::exportProperties(Id const&id, QDomDocument &doc, QDomElement &root, QHash<Id, Object *> const &objects)
{
	QDomElement props = doc.createElement("properties");

	GraphicalObject const * const graphicalObject = dynamic_cast<GraphicalObject const *>(objects[id]);
	LogicalObject const * const logicalObject
			= dynamic_cast<LogicalObject const *>(objects[graphicalObject->logicalId()]);

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

	foreach (QString const &key, properties.keys()) {
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
