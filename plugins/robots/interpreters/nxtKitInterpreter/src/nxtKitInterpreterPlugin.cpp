#include "nxtKitInterpreterPlugin.h"

using namespace nxtKitInterpreter;

QString NxtKitInterpreterPlugin::kitId() const
{
	return "nxtKit";
}

QString NxtKitInterpreterPlugin::friendlyKitName() const
{
	return tr("Empty Kit");
}

QList<interpreterBase::robotModel::RobotModelInterface *> NxtKitInterpreterPlugin::robotModels()
{
	return QList<interpreterBase::robotModel::RobotModelInterface *>()
			<< &mRealRobotModel << &mTwoDRobotModel;
}

QWidget *NxtKitInterpreterPlugin::settingsWidget() const
{
	return nullptr;
}

//qReal::IdList NxtKitInterpreterInterface::specificBlocks() const
//{
//	return qReal::IdList();
//}

qReal::IdList NxtKitInterpreterPlugin::unsupportedBlocks() const
{
	return qReal::IdList();
}
