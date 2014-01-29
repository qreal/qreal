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

QWidget *NullKitInterpreterPlugin::settingsWidget() const
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
