#pragma once

#include <gtest/gtest.h>

#include "../../../qrrepo/private/repository.h"
#include "../../../qrrepo/private/serializer.h"

namespace qrTest {

class RepositoryTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();

	void removeDirectory(QString const &dirName);

	qrRepo::details::Repository *mRepository;
	qrRepo::details::Serializer *mSerializer;

	QString mOldTempFolder;
	QString mNewTempFolder;
};

}
