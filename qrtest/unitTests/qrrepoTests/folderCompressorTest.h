#pragma once

#include "../../../qrrepo/private/folderCompressor.h"

#include "gtest/gtest.h"

namespace qrTest {

class FolderCompressorTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();
	
	void removeDirectory(QString const &dirName);

	FolderCompressor *mFolderCompressor;
};

}
