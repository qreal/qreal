#pragma once

#include "../../qrrepo/private/client.h"
#include "../../qrrepo/private/serializer.h"
#include "../../qrkernel/ids.h"

#include "gtest/gtest.h"

namespace qrTest {

class ClientTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();
	
	void removeDirectory(QString const &dirName);

	qrRepo::details::Client *mClient;
	qrRepo::details::Serializer *mSerializer;
};

}
