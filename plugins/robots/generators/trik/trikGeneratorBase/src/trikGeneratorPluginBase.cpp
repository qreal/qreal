#include "trikGeneratorBase/trikGeneratorPluginBase.h"

#include <trikKit/blocks/trikBlocksFactory.h>

#include "src/robotModel/trikGeneratorRobotModel.h"

using namespace trik;

TrikGeneratorPluginBase::TrikGeneratorPluginBase(const QString &robotName
		, const QString &robotFriendlyName
		, int priority)
	: mRobotModel(new robotModel::TrikGeneratorRobotModel(kitId()
			, "trikGeneratorRobotOf" + robotName, robotName, robotFriendlyName, priority))
	, mBlocksFactory(new blocks::TrikBlocksFactory)
{
}

TrikGeneratorPluginBase::~TrikGeneratorPluginBase()
{
}

QString TrikGeneratorPluginBase::kitId() const
{
	return "trikKit";
}

QList<kitBase::robotModel::RobotModelInterface *> TrikGeneratorPluginBase::robotModels()
{
	return { mRobotModel.data() };
}

kitBase::blocksBase::BlocksFactoryInterface *TrikGeneratorPluginBase::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model)
	return mBlocksFactory;
}

QList<kitBase::AdditionalPreferences *> TrikGeneratorPluginBase::settingsWidgets()
{
	return {};
}

void TrikGeneratorPluginBase::regenerateExtraFiles(const QFileInfo &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}
