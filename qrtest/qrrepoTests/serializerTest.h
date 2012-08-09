#pragma once

#include "../../qrrepo/private/serializer.h"

#include "gtest/gtest.h"

namespace qrTest {

class SerializerTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();
	
	void removeDirectory(QString const &dirName);

	qrRepo::details::Serializer *mSerializer;
};

}
