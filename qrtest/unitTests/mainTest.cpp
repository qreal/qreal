#include <gtest/gtest.h>

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	QApplication app(argc, argv);

	Q_UNUSED(app);

	return RUN_ALL_TESTS();
}
