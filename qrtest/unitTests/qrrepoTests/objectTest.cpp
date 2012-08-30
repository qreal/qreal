#include "../../../qrrepo/private/classes/object.h"

#include "gtest/gtest.h"

using namespace qReal;

TEST(ObjectTest, replacePropertiesTest) {
	Id const id("editor", "diagram", "element", "id");
	qrRepo::details::Object obj(id);
	obj.setProperty("testProperty1", "value1");
	obj.setProperty("property", "val");
	obj.setProperty("property_test2", "value2");
	obj.replaceProperties("value", "replace_value");
	
	EXPECT_EQ(obj.property("testProperty1").toString(), "replace_value");
	EXPECT_EQ(obj.property("property_test2").toString(), "replace_value");
	EXPECT_EQ(obj.property("property").toString(), "val");
}
