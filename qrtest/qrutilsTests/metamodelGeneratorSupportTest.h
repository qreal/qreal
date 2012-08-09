#pragma once

#include "../../qrutils/metamodelGeneratorSupport.h"

#include "gtest/gtest.h"

namespace qrTest {

class MetamodelGeneratorSupportTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();

	utils::MetamodelGeneratorSupport *mGenerator;
	QDomDocument mDocument;
};

}
