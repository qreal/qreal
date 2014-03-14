#include "nullKitInterpreterPlugin.h"

using namespace nullKitInterpreter;

QString NullKitInterpreterPlugin::kitId() const
{
	return "nullKit";
}

QString NullKitInterpreterPlugin::friendlyKitName() const
{
	return tr("Empty Kit");
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

//QList<QWidget *> NullKitInterpreterInterface::toolWidgets() const
//{
//	return QList<QWidget *>();
//}

//QList<robotsInterpreterCore::SensorId> NullKitInterpreterInterface::sensorIds() const
//{
//	return QList<robotsInterpreterCore::SensorId>();
//}

//qReal::IdList NullKitInterpreterInterface::specificBlocks() const
//{
//	return qReal::IdList();
//}

qReal::IdList NullKitInterpreterPlugin::unsupportedBlocks() const
{
	return qReal::IdList();
}

QList<qReal::ActionInfo> NullKitInterpreterPlugin::customActions()
{
	return {};
}
