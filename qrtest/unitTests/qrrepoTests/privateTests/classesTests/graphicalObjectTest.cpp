#include "../../../qrrepo/private/classes/graphicalObject.h"

#include <QtCore/QPointF>
#include <gtest/gtest.h>

using namespace qReal;
using namespace qrRepo::details;

TEST(GraphicalObjectTest, graphicalPartsCloneTest)
{
	Id const element("editor", "diagram", "element", "id");
	Id const graphicalElement("editor", "diagram", "element", "graphicalId");

	GraphicalObject graphicalObj(graphicalElement, Id(), element);

	QHash<Id, Object *> objHash;
	objHash.insert(graphicalElement, &graphicalObj);

	graphicalObj.createGraphicalPart(0);
	graphicalObj.setGraphicalPartProperty(0, "Coord", QPointF(10, 20));

	Object const * const cloned = graphicalObj.clone(objHash);

	GraphicalObject const * const graphicalClone = dynamic_cast<GraphicalObject const *>(cloned);

	ASSERT_TRUE(graphicalClone != NULL);
	ASSERT_EQ(QPointF(10, 20), graphicalClone->graphicalPartProperty(0, "Coord"));
}

TEST(GraphicalObjectTest, graphicalPartsSerializeTest)
{
	Id const element("editor", "diagram", "element", "id");
	Id const graphicalElement("editor", "diagram", "element", "graphicalId");

	GraphicalObject graphicalObject(graphicalElement, Id(), element);

	graphicalObject.createGraphicalPart(0);
	graphicalObject.setGraphicalPartProperty(0, "Coord", QPointF(10, 20));

	QDomDocument document;
	QDomElement serializedGraphicalObject = graphicalObject.serialize(document);
	document.appendChild(serializedGraphicalObject);

	ASSERT_EQ("qrm:/editor/diagram/element/graphicalId", serializedGraphicalObject.attribute("id").toStdString());
	ASSERT_EQ("qrm:/editor/diagram/element/id", serializedGraphicalObject.attribute("logicalId").toStdString());
	ASSERT_EQ("qrm:/", serializedGraphicalObject.attribute("parent").toStdString());
	ASSERT_TRUE(serializedGraphicalObject.firstChildElement("children").childNodes().isEmpty());
	ASSERT_TRUE(serializedGraphicalObject.firstChildElement("properties").childNodes().isEmpty());
	ASSERT_FALSE(serializedGraphicalObject.firstChildElement("graphicalParts").childNodes().isEmpty());

	QDomElement graphicalPart
			= serializedGraphicalObject.firstChildElement("graphicalParts").firstChildElement("graphicalPart");

	ASSERT_EQ("0", graphicalPart.attribute("index").toStdString());
	ASSERT_EQ("QPointF", graphicalPart.firstChildElement().tagName().toStdString());
	ASSERT_EQ("Coord", graphicalPart.firstChildElement().attribute("key").toStdString());
	ASSERT_EQ("10, 20", graphicalPart.firstChildElement().attribute("value").toStdString());
}
