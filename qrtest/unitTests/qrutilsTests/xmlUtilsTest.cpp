#include <QtCore/QFile>

#include "../../../qrutils/xmlUtils.h"

#include "gtest/gtest.h"

TEST(XmlUtilsTest, loadDocumentTest) {
	QFile file("testMetamodelFile");
	file.open(QIODevice::ReadWrite);

	file.write("<diagram>");
	file.write("<graphicTypes>");
	file.write("</graphicTypes>");
	file.write("<logicTypes>");
	file.write("</logicTypes>");
	file.write("</diagram>");
	file.close();

	QDomDocument doc = utils::xmlUtils::loadDocument("testMetamodelFile");
	QFile::remove("testMetamodelFile");

	ASSERT_EQ(doc.elementsByTagName("diagram").size(), 1);
	ASSERT_EQ(doc.elementsByTagName("graphicTypes").size(), 1);
	ASSERT_EQ(doc.elementsByTagName("logicTypes").size(), 1);
	ASSERT_EQ(doc.elementsByTagName("diagram").at(0).childNodes().size(), 2);
}

