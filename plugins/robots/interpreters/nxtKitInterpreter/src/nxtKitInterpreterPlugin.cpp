#include "nxtKitInterpreterPlugin.h"

using namespace nxtKitInterpreter;

NxtKitInterpreterPlugin::NxtKitInterpreterPlugin()
	: mAdditionalPreferences(new NxtAdditionalPreferences(mRealRobotModel.name()))
{
}

QString NxtKitInterpreterPlugin::kitId() const
{
	return "nxtKit";
}

QString NxtKitInterpreterPlugin::friendlyKitName() const
{
	return tr("Lego NXT");
}

QList<interpreterBase::robotModel::RobotModelInterface *> NxtKitInterpreterPlugin::robotModels()
{
	return QList<interpreterBase::robotModel::RobotModelInterface *>()
			<< &mRealRobotModel << &mTwoDRobotModel;
}

interpreterBase::robotModel::RobotModelInterface *NxtKitInterpreterPlugin::defaultRobotModel()
{
	return &mTwoDRobotModel;
}

interpreterBase::AdditionalPreferences *NxtKitInterpreterPlugin::settingsWidget()
{
	return mAdditionalPreferences;
}

//qReal::IdList NxtKitInterpreterInterface::specificBlocks() const
//{
//	return qReal::IdList();
//}

qReal::IdList NxtKitInterpreterPlugin::unsupportedBlocks() const
{
	return qReal::IdList();
}
