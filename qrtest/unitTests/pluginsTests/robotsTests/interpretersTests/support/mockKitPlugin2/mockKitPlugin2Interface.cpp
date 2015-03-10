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

QList<kitBase::robotModel::RobotModelInterface *> MockKitPlugin2Interface::robotModels()
{
	return QList<kitBase::robotModel::RobotModelInterface *>();
}

QList<kitBase::AdditionalPreferences *> MockKitPlugin2Interface::settingsWidgets()
{
	return {nullptr};
}

kitBase::blocksBase::BlocksFactoryInterface *MockKitPlugin2Interface::blocksFactoryFor(
		kitBase::robotModel::RobotModelInterface const *model)
{
	Q_UNUSED(model)
	return nullptr;
}

QList<qReal::ActionInfo> MockKitPlugin2Interface::customActions()
{
	return {};
}

QList<qReal::HotKeyActionInfo> MockKitPlugin2Interface::hotKeyActions()
{
	return {};
}

QIcon MockKitPlugin2Interface::iconForFastSelector(
		kitBase::robotModel::RobotModelInterface const &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon();
}
