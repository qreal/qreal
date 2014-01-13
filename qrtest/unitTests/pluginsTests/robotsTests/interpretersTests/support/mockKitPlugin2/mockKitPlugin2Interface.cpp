#include "mockKitPlugin2Interface.h"

using namespace qrTest::mockKitPlugin2;

QString MockKitPlugin2Interface::kitId() const
{
	return "MockKitPlugin2";
}

QString MockKitPlugin2Interface::friendlyKitName() const
{
	return "Mock Kit Plugin 2";
}

interpreterBase::robotModel::RobotModelInterface &MockKitPlugin2Interface::robotModel()
{
	return mRobotModel;
}

QWidget *MockKitPlugin2Interface::settingsWidget() const
{
	return nullptr;
}

qReal::IdList MockKitPlugin2Interface::unsupportedBlocks() const
{
	return {};
}
