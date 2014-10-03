#pragma once

#include <gtest/gtest.h>
#include <models/details/graphicalPartModel.h>
#include <../qrrepo/repoApi.h>

#include <mocks/qrgui/models/details/modelsImplementation/modelIndexesInterfaceMock.h>

namespace qrguiTests {

class GraphicalPartModelTest : public testing::Test {

protected:
	virtual void SetUp();
	virtual void TearDown();

protected:
	qReal::models::details::GraphicalPartModel *mGraphicalPartModel;
	qrRepo::RepoApi *mRepoApi;
	qrTest::ModelIndexesInterfaceMock mModelIndexesInterfaceMock;
};

}
