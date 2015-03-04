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

QList<kitBase::robotModel::RobotModelInterface *> MockKitPlugin1Interface::robotModels()
{
	return QList<kitBase::robotModel::RobotModelInterface *>();
}

QList<kitBase::AdditionalPreferences *>  MockKitPlugin1Interface::settingsWidgets()
{
	return {nullptr};
}

kitBase::blocksBase::BlocksFactoryInterface *MockKitPlugin1Interface::blocksFactoryFor(
		kitBase::robotModel::RobotModelInterface const *model)
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
		kitBase::robotModel::RobotModelInterface const &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon();
}
