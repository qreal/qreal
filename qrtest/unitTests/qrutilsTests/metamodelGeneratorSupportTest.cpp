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

#include <QtCore/QFile>

#include "metamodelGeneratorSupportTest.h"
#include "../../../qrutils/inFile.h"

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

	QDomNodeList const containsElems = mDocument.elementsByTagName("container").at(0).childNodes();
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
	QDomElement const elem = mDocument.createElement("TestElement");
	mGenerator->insertElementInDiagramSublevel(mDocument ,"nonGraphicTypes", elem);

	ASSERT_EQ(mGenerator->diagramElement(mDocument).childNodes().size(), 2);
	ASSERT_EQ(mDocument.elementsByTagName("TestElement").size(), 1);
}

TEST_F(MetamodelGeneratorSupportTest, insertElementsInDiagramSublevelTest) {
	QDomDocument doc = mGenerator->loadElementsFromString("<diagram><testElem1/><testElem2/><testElem3/><></diagram>");

	mGenerator->insertElementsInDiagramSublevel(mDocument ,"nonGraphicTypes"
			, mGenerator->diagramElement(doc).childNodes());

	ASSERT_EQ(mGenerator->diagramElement(mDocument).childNodes().size(), 2);
	ASSERT_EQ(mDocument.elementsByTagName("nonGraphicTypes").size(), 1);

	ASSERT_EQ(mDocument.elementsByTagName("nonGraphicTypes").at(0).childNodes().size(), 3);
	ASSERT_EQ(mDocument.elementsByTagName("testElem1").size(), 1);
	ASSERT_EQ(mDocument.elementsByTagName("testElem2").size(), 1);
	ASSERT_EQ(mDocument.elementsByTagName("testElem3").size(), 1);
}

TEST_F(MetamodelGeneratorSupportTest, appendElementsTest) {
	QDomDocument doc = mGenerator->loadElementsFromString("<diagram><testElem1/><testElem2/><testElem3/><></diagram>");

	mGenerator->appendElements(mGenerator->diagramElement(mDocument), mGenerator->diagramElement(doc).childNodes());

	ASSERT_EQ(mGenerator->diagramElement(mDocument).childNodes().size(), 4);
	ASSERT_EQ(mDocument.elementsByTagName("testElem1").size(), 1);
	ASSERT_EQ(mDocument.elementsByTagName("testElem2").size(), 1);
	ASSERT_EQ(mDocument.elementsByTagName("testElem3").size(), 1);
}

TEST_F(MetamodelGeneratorSupportTest, diagramElementTest) {
	QDomElement const elem = mGenerator->diagramElement(mDocument);

	ASSERT_EQ(elem.nodeName(), "diagram");
}

TEST_F(MetamodelGeneratorSupportTest, loadElementsFromStringTest) {
	ASSERT_EQ(mDocument.elementsByTagName("diagram").size(), 1);
	ASSERT_EQ(mDocument.elementsByTagName("graphicTypes").size(), 1);
	ASSERT_EQ(mDocument.elementsByTagName("node").size(), 3);
	ASSERT_EQ(mDocument.elementsByTagName("edge").size(), 2);
	ASSERT_EQ(mDocument.elementsByTagName("labels").size(), 2);
	ASSERT_EQ(mDocument.elementsByTagName("container").size(), 1);
	ASSERT_EQ(mDocument.elementsByTagName("property").size(), 3);
}

TEST_F(MetamodelGeneratorSupportTest, loadAndSaveMetamodelInFileTest) {
	QFile file("testMetamodelFile");
	file.open(QIODevice::ReadWrite);

	file.write("<diagram>");
	file.write("<graphicTypes>");
	file.write("</graphicTypes>");
	file.write("<logicTypes>");
	file.write("</logicTypes>");
	file.write("</diagram>");
	file.close();

	QDomDocument metamodel = mGenerator->loadMetamodelFromFile("testMetamodelFile");
	QFile::remove("testMetamodelFile");

	ASSERT_EQ(metamodel.elementsByTagName("diagram").size(), 1);
	ASSERT_EQ(metamodel.elementsByTagName("graphicTypes").size(), 1);
	ASSERT_EQ(metamodel.elementsByTagName("logicTypes").size(), 1);
	ASSERT_EQ(metamodel.elementsByTagName("diagram").at(0).childNodes().size(), 2);

	mGenerator->saveMetamodelInFile(metamodel, "savedTestMetamodel");
	QString const savedData = InFile::readAll("savedTestMetamodel");

	QFile::remove("savedTestMetamodel");

	ASSERT_EQ(savedData, "<diagram>\n <graphicTypes/>\n <logicTypes/>\n</diagram>\n");
}
