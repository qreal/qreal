#include "mockKitPlugin1Interface.h"

using namespace qrTest::mockKitPlugin1;

QString MockKitPlugin1Interface::kitId() const
{
	return "MockKitPlugin1";
}

QString MockKitPlugin1Interface::friendlyKitName() const
{
	return "Mock Kit Plugin 1";
}

QList<interpreterBase::robotModel::RobotModelInterface *> MockKitPlugin1Interface::robotModels()
{
	return QList<interpreterBase::robotModel::RobotModelInterface *>();
}

interpreterBase::AdditionalPreferences *MockKitPlugin1Interface::settingsWidget()
{
	return nullptr;
}

interpreterBase::blocksBase::BlocksFactoryInterface *MockKitPlugin1Interface::blocksFactoryFor(
		interpreterBase::robotModel::RobotModelInterface const *model)
{
	Q_UNUSED(model)
	return nullptr;
}

QList<qReal::ActionInfo> MockKitPlugin1Interface::customActions()
{
	return {};
}

QList<qReal::HotKeyActionInfo> MockKitPlugin1Interface::hotKeyActions()
{
	return {};
}

QIcon MockKitPlugin1Interface::iconForFastSelector(
		interpreterBase::robotModel::RobotModelInterface const &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon();
}
