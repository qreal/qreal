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
