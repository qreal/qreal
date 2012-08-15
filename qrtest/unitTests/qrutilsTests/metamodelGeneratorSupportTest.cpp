#include "metamodelGeneratorSupportTest.h"

#include "gtest/gtest.h"

using namespace utils;
using namespace qrTest;

void MetamodelGeneratorSupportTest::SetUp() {
	mGenerator = new MetamodelGeneratorSupport(NULL, NULL);

	QString const elementsXml =
	"<diagram>"
	"<graphicTypes>"
	"<node displayedName=\"Semantics Rule\" name=\"SemanticsRule\">"
		"<graphics>"
			"<labels>"
				"<label x=\"0\" y=\"0\" textBinded=\"ruleName\"/>"
				"<label x=\"0\" y=\"10\" textBinded=\"procedure\"/>"
			"</labels>"
		"</graphics>"
		"<logic>"
			"<properties>"
				"<property type=\"string\" name=\"ruleName\" />"
				"<property type=\"string\" name=\"procedure\" />"
			"</properties>"
			"<container>"
			"</container>"
		"</logic>"
	"</node>"
	"<node displayedName=\"Wildcard\" name=\"Wildcard\">"
		"<graphics>"
			"<ports>"
				"<pointPort x=\"10\" y=\"10\"/>"
			"</ports>"
		"</graphics>"
		"<logic>"
		"</logic>"
	"</node>"
	"<node displayedName=\"Control Flow Mark\" name=\"ControlFlowMark\">"
		"<graphics>"
			"<ports>"
				"<pointPort x=\"0\" y=\"0\"/>"
			"</ports>"
			"<labels>"
				"<label x=\"10\" y=\"0\" textBinded=\"semanticsStatus\"/>"
			"</labels>"
		"</graphics>"
		"<logic>"
			"<properties>"
				"<property type=\"SemanticsStatus\" name=\"semanticsStatus\"/>"
			"</properties>"
		"</logic>"
	"</node>"
	"<edge displayedName=\"Replacement\" name=\"Replacement\">"
		"<graphics>"
			"<lineType type=\"dashLine\"/>"
		"</graphics>"
		"<logic>"
			"<associations endType=\"open_arrow\" beginType=\"no_arrow\">"
			"</associations>"
		"</logic>"
	"</edge>"
	"<edge displayedName=\"Control Flow Location\" name=\"ControlFlowLocation\">"
		"<graphics>"
			"<lineType type=\"dotLine\"/>"
		"</graphics>"
		"<logic>"
			"<associations endType=\"open_arrow\" beginType=\"no_arrow\">"
			"</associations>"
		"</logic>"
	"</edge>"
	"</graphicTypes>"
	"</diagram>";

	mDocument = mGenerator->loadElementsFromString(elementsXml);
}

void MetamodelGeneratorSupportTest::TearDown() {
	delete mGenerator;
}

TEST_F(MetamodelGeneratorSupportTest, collectAllGraphicTypesInMetamodelTest) {
	QStringList types = mGenerator->collectAllGraphicTypesInMetamodel(mDocument, true);

	ASSERT_EQ(types.size(), 5);
	EXPECT_TRUE(types.contains("Semantics Rule"));
	EXPECT_TRUE(types.contains("Wildcard"));
	EXPECT_TRUE(types.contains("Control Flow Mark"));
	EXPECT_TRUE(types.contains("Replacement"));
	EXPECT_TRUE(types.contains("Control Flow Location"));

	types = mGenerator->collectAllGraphicTypesInMetamodel(mDocument, false);

	ASSERT_EQ(types.size(), 5);
	EXPECT_TRUE(types.contains("SemanticsRule"));
	EXPECT_TRUE(types.contains("Wildcard"));
	EXPECT_TRUE(types.contains("ControlFlowMark"));
	EXPECT_TRUE(types.contains("Replacement"));
	EXPECT_TRUE(types.contains("ControlFlowLocation"));
}

TEST_F(MetamodelGeneratorSupportTest, appendTypesToElementTest) {
	QDomElement const container = mDocument.elementsByTagName("container").at(0).toElement();
	QStringList elementNames = mGenerator->collectAllGraphicTypesInMetamodel(mDocument, false);

	mGenerator->appendTypesToElement(mDocument, container, "contains", "TestDiagram", elementNames);

	QDomNodeList containsElems = mDocument.elementsByTagName("container").at(0).childNodes();
	ASSERT_EQ(containsElems.size(), 5);

	QStringList containsElemNames;
	for (int i = 0; i < containsElems.size(); i++) {
		containsElemNames << containsElems.at(i).toElement().attribute("type");
	}
	EXPECT_TRUE(containsElemNames.contains("TestDiagram::SemanticsRule"));
	EXPECT_TRUE(containsElemNames.contains("TestDiagram::Wildcard"));
	EXPECT_TRUE(containsElemNames.contains("TestDiagram::ControlFlowMark"));
	EXPECT_TRUE(containsElemNames.contains("TestDiagram::Replacement"));
	EXPECT_TRUE(containsElemNames.contains("TestDiagram::ControlFlowLocation"));
}

TEST_F(MetamodelGeneratorSupportTest, insertElementInDiagramSublevelTest) {
	QDomElement elem = mDocument.createElement("TestElement");
	mGenerator->insertElementInDiagramSublevel(mDocument ,"nonGraphicTypes", elem);

	ASSERT_EQ(mGenerator->diagramElement(mDocument).childNodes().size(), 2);
	ASSERT_EQ(mDocument.elementsByTagName("TestElement").size(), 1);
}
