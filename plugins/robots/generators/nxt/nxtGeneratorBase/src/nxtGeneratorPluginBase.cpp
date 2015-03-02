#include "nxtGeneratorBase/nxtGeneratorPluginBase.h"

#include <nxtKit/blocks/nxtBlocksFactory.h>

#include "src/robotModel/nxtGeneratorRobotModel.h"

using namespace nxt;

NxtGeneratorPluginBase::NxtGeneratorPluginBase(const QString &robotName, const QString &robotFriendlyName)
	: mRobotModel(new robotModel::NxtGeneratorRobotModel(kitId(), "nxtGeneratorRobot", robotName, robotFriendlyName))
	, mBlocksFactory(new blocks::NxtBlocksFactory)
{
}

NxtGeneratorPluginBase::~NxtGeneratorPluginBase()
{
}

QString NxtGeneratorPluginBase::kitId() const
{
	return "nxtKit";
}

QList<kitBase::robotModel::RobotModelInterface *> NxtGeneratorPluginBase::robotModels()
{
	return { mRobotModel.data() };
}

kitBase::blocksBase::BlocksFactoryInterface *NxtGeneratorPluginBase::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model)
	return mBlocksFactory;
}

QList<kitBase::AdditionalPreferences *> NxtGeneratorPluginBase::settingsWidgets()
{
	return {};
}
