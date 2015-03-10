#include "nullKitInterpreterPlugin.h"

#include <QtWidgets/QApplication>

using namespace nullKitInterpreter;

NullKitInterpreterPlugin::NullKitInterpreterPlugin()
	: mRobotModel(kitId())
{
}

QString NullKitInterpreterPlugin::kitId() const
{
	return "nullKit";
}

QString NullKitInterpreterPlugin::friendlyKitName() const
{
	return tr("Empty Kit");
}

QList<kitBase::robotModel::RobotModelInterface *> NullKitInterpreterPlugin::robotModels()
{
	return QList<kitBase::robotModel::RobotModelInterface *>() << &mRobotModel;
}

kitBase::blocksBase::BlocksFactoryInterface *NullKitInterpreterPlugin::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model)
	return nullptr;
}

QList<kitBase::AdditionalPreferences *> NullKitInterpreterPlugin::settingsWidgets()
{
	return {nullptr};
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
		const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon();
}
