#include "nxtKitInterpreterPlugin.h"

using namespace nxtKitInterpreter;

NxtKitInterpreterPlugin::NxtKitInterpreterPlugin()
	: mAdditionalPreferences(new NxtAdditionalPreferences(mRealRobotModel.name()))
	, mTwoDModelAction(QIcon(":/icons/2d-model.svg"), QObject::tr("2d model"), nullptr)
{
	connect(mAdditionalPreferences, &NxtAdditionalPreferences::settingsChanged
			, &mRealRobotModel, &robotModel::real::RealRobotModel::rereadSettings);
	connect(mAdditionalPreferences, &NxtAdditionalPreferences::settingsChanged
			, &mTwoDRobotModel, &robotModel::twoD::TwoDRobotModel::rereadSettings);

	/// @todo: why it can't be done in constructor?
	mTwoDModel.createModelWidget();
	connect(&mTwoDModelAction, &QAction::triggered, &mTwoDModel, &twoDModel::D2RobotModel::showModelWidget);
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
	return {&mRealRobotModel, &mTwoDRobotModel};
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
	return {};
}

QList<qReal::ActionInfo> NxtKitInterpreterPlugin::customActions()
{
	qReal::ActionInfo twoDModelActionInfo(&mTwoDModelAction, "interpreters", "tools");

	return {twoDModelActionInfo};
}
