#include "nxtKitInterpreterInterface.h"

using namespace nxtKitInterpreter;

QString NxtKitInterpreterInterface::kitId() const
{
	return "nxtKit";
}

QString NxtKitInterpreterInterface::friendlyKitName() const
{
	return tr("Empty Kit");
}

interpreterBase::robotModel::RobotModelInterface *NxtKitInterpreterInterface::realRobotModel()
{
	return nullptr;
}

interpreterBase::robotModel::RobotModelInterface *NxtKitInterpreterInterface::twoDRobotModel()
{
	return nullptr;
}

QWidget *NxtKitInterpreterInterface::settingsWidget() const
{
	return nullptr;
}

//qReal::IdList NxtKitInterpreterInterface::specificBlocks() const
//{
//	return qReal::IdList();
//}

qReal::IdList NxtKitInterpreterInterface::unsupportedBlocks() const
{
	return qReal::IdList();
}
