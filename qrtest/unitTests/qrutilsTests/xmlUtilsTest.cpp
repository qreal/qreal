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

