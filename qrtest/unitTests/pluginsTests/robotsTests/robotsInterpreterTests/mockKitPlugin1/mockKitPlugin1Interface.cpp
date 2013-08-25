#include "mockKitPlugin1Interface.h"

using namespace qrTest::mockKitPlugin1;

QString MockKitPlugin1Interface::kitId() const
{
	return "MockKitPlugin1";
}

QWidget *MockKitPlugin1Interface::settingsWidget() const
{
	return NULL;
}

qReal::IdList MockKitPlugin1Interface::unsupportedBlocks() const
{
	return qReal::IdList();
}
