#include "../../../../qrkernel/exception/exception.h"

#include "gtest/gtest.h"

using namespace qReal;

TEST(ExceptionTest, messageTest) {
	QString const msg = "Test error message";
	Exception ex(msg);
	EXPECT_EQ(ex.message(), msg);
}
