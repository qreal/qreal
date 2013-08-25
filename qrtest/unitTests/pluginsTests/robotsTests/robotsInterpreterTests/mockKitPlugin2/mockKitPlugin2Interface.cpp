#include "mockKitPlugin2Interface.h"

using namespace qrTest::mockKitPlugin2;

QString MockKitPlugin2Interface::kitId() const
{
	return "MockKitPlugin2";
}

QWidget *MockKitPlugin2Interface::settingsWidget() const
{
	return NULL;
}

qReal::IdList MockKitPlugin2Interface::unsupportedBlocks() const
{
	return qReal::IdList();
}
