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
#include <QtCore/QVariant>
#include <QtCore/QDataStream>

#include <qrkernel/ids.h>

#include "gtest/gtest.h"

using namespace qReal;

TEST(IdsTest, loadFromStringTest) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	EXPECT_DEATH_IF_SUPPORTED(Id::loadFromString("qrm:/editor/diagram/element/id/test")
			, ".*path\\.count\\(\\) > 0 && path\\.count\\(\\) <= 5.*");

	EXPECT_DEATH_IF_SUPPORTED(Id::loadFromString("qm:/editor/diagram/element/id")
			, ".*path\\[0\\] == \"qrm:\".*");

	EXPECT_DEATH_IF_SUPPORTED(Id::loadFromString("qrm:/editor//diagram")
			, ".*string == result\\.toString\\(\\).*");

	Id id = Id::loadFromString("qrm:/editor/diagram/element/id");
	EXPECT_EQ(id, Id("editor", "diagram", "element", "id"));

	id = Id::loadFromString("qrm:/editor/diagram/element");
	EXPECT_EQ(id, Id("editor", "diagram", "element", ""));

	id = Id::loadFromString("qrm:/editor/diagram");
	EXPECT_EQ(id, Id("editor", "diagram", "", ""));

	id = Id::loadFromString("qrm:/editor");
	EXPECT_EQ(id, Id("editor", "", "", ""));

	id = Id::loadFromString("qrm:/");
	EXPECT_EQ(id, Id("", "", "", ""));
}

TEST(IdsTest, checkIntegrityTest) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	EXPECT_DEATH_IF_SUPPORTED(Id id("editor", "diagram", "", "id")
			, ".*checkIntegrity\\(\\).*");

	EXPECT_DEATH_IF_SUPPORTED(Id id("editor", "", "element", "id")
			, ".*checkIntegrity\\(\\).*");

	EXPECT_DEATH_IF_SUPPORTED(Id id("", "diagram", "element", "id")
			, ".*checkIntegrity\\(\\).*");

	EXPECT_DEATH_IF_SUPPORTED(Id id("editor", "", "", "id")
			, ".*checkIntegrity\\(\\).*");
}

TEST(IdsTest, createElementIdTest) {
	Id id = Id::createElementId("diagram", "editor", "element");
	EXPECT_TRUE(id.id() != "");
}

TEST(IdsTest, rootIdTest) {
	Id id = Id::rootId();
	EXPECT_EQ(id, Id("ROOT_ID", "ROOT_ID", "ROOT_ID", "ROOT_ID"));
}

TEST(IdsTest, additonalConstructorTest) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	EXPECT_DEATH_IF_SUPPORTED(Id id(Id::loadFromString("qrm:/editor/diagram/element/id"), "test")
			, ".*Can not add a part to Id, it will be too long.*");

	Id id1(Id::loadFromString("qrm:/editor/diagram/element"), "id");
	EXPECT_EQ(id1.id(), "id");

	Id id2(Id::loadFromString("qrm:/editor/diagram"), "element");
	EXPECT_EQ(id2.element(), "element");

	Id id3(Id::loadFromString("qrm:/editor"), "diagram");
	EXPECT_EQ(id3.diagram(), "diagram");

	Id id4(Id::loadFromString("qrm:/"), "editor");
	EXPECT_EQ(id4.editor(), "editor");
}

TEST(IdsTest, gettersTest) {
	Id id = Id::loadFromString("qrm:/editor/diagram/element/id");

	EXPECT_EQ(id.editor(), "editor");
	EXPECT_EQ(id.diagram(), "diagram");
	EXPECT_EQ(id.element(), "element");
	EXPECT_EQ(id.id(), "id");
	EXPECT_EQ(id.type() ,Id("editor", "diagram", "element"));
}

TEST(IdsTest, idSizeTest) {
	Id id = Id::loadFromString("qrm:/editor/diagram/element/id");
	EXPECT_EQ(id.idSize(), (uint) 4);

	id = Id::loadFromString("qrm:/editor/diagram/element");
	EXPECT_EQ(id.idSize(), (uint) 3);

	id = Id::loadFromString("qrm:/editor/diagram");
	EXPECT_EQ(id.idSize(), (uint) 2);

	id = Id::loadFromString("qrm:/editor");
	EXPECT_EQ(id.idSize(), (uint) 1);

	id = Id::loadFromString("qrm:/");
	EXPECT_EQ(id.idSize(), (uint) 0);
}

TEST(IdsTest, sameTypeIdTest) {
	Id const id = Id::loadFromString("qrm:/editor/diagram/element/id");
	EXPECT_EQ(id.sameTypeId().type(), id.type());
}


TEST(IdsTest, toUrlToStringToVariantTest) {
	QString const idString = "qrm:/editor/diagram/element/id";
	Id const id = Id::loadFromString(idString);

	EXPECT_EQ(id.toString(), idString);
	EXPECT_EQ(id.toUrl(), QUrl(idString));

	QVariant toVariant = id.toVariant();
	EXPECT_EQ(toVariant.value<Id>(), id);
}

TEST(IdsTest, idInputOutputStreamTest) {
	QString const idString = "qrm:/editor/diagram/element/id";
	Id const out = Id::loadFromString(idString);

	QFile fileOut("test");
	fileOut.open(QIODevice::WriteOnly);
	QDataStream outputStream(&fileOut);

	outputStream << out;
	fileOut.close();

	QFile fileIn("test");
	fileIn.open(QIODevice::ReadOnly);
	QDataStream inputStream(&fileIn);

	Id in;
	inputStream >> in;

	fileIn.close();
	QFile::remove("test");

	EXPECT_EQ(in, out);
}
