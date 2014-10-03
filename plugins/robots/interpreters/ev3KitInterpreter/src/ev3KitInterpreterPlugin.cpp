#include "ev3KitInterpreterPlugin.h"

#include <QtWidgets/QApplication>

using namespace ev3KitInterpreter;

NullKitInterpreterPlugin::NullKitInterpreterPlugin()
	: mRobotModel(kitId())
{
}

QString NullKitInterpreterPlugin::kitId() const
{
	return "ev3Kit";
}

QString NullKitInterpreterPlugin::friendlyKitName() const
{
	return tr("EV3 - comming soon!");
}

QList<interpreterBase::robotModel::RobotModelInterface *> NullKitInterpreterPlugin::robotModels()
{
	return QList<interpreterBase::robotModel::RobotModelInterface *>() << &mRobotModel;
}

interpreterBase::blocksBase::BlocksFactoryInterface *NullKitInterpreterPlugin::blocksFactoryFor(
		interpreterBase::robotModel::RobotModelInterface const *model)
{
	Q_UNUSED(model)
	return nullptr;
}

interpreterBase::AdditionalPreferences *NullKitInterpreterPlugin::settingsWidget()
{
	return nullptr;
}

QList<qReal::ActionInfo> NullKitInterpreterPlugin::customActions()
{
	return {};
}

QList<qReal::HotKeyActionInfo> NullKitInterpreterPlugin::hotKeyActions()
{
	return {};
}

QIcon NullKitInterpreterPlugin::iconForFastSelector(
		interpreterBase::robotModel::RobotModelInterface const &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon();
}
