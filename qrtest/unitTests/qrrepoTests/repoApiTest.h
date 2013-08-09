#pragma once

#include <gtest/gtest.h>

#include "../../../qrrepo/repoApi.h"

namespace qrTest {

class RepoApiTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();

protected:
	qrRepo::RepoApi *mRepoApi;
};

}
