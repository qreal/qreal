#include "../../../qrkernel/ids.h"

#include "gtest/gtest.h"

using namespace qReal;

TEST(IdsTest, loadFromStringTest) {
	Id const id = Id::loadFromString("qrm:/editor/diagram/element/id");
	EXPECT_EQ(id, Id("editor", "diagram", "element", "id"));
}

TEST(IdsTest, additonalConstructorTest) {
	Id id(Id::loadFromString("qrm:/editor/diagram"), "element");
	EXPECT_EQ(id.element(), "element");
}

TEST(IdsTest, sameTypeIdTest) {
	Id const id = Id::loadFromString("qrm:/editor/diagram/element/id");
	EXPECT_EQ(id.sameTypeId().type(), id.type());
}

TEST(IdsTest, toStringTest) {
	QString const idString = "qrm:/editor/diagram/element/id";
	Id const id = Id::loadFromString(idString);
	EXPECT_EQ(id.toString(), idString);
}
