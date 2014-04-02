#include "commonTwoDModel/engine/twoDModelEngineFacade.h"

#include "src/engine/d2RobotModel.h"

using namespace twoDModel::engine;

TwoDModelEngineFacade::TwoDModelEngineFacade(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: mTwoDModelActionInfo(
			new QAction(QIcon(":/icons/2d-model.svg"), QObject::tr("2d model"), nullptr)
			, "interpreters"
			, "tools")
	, mTwoDModel(new D2RobotModel(robotModel))
{
	/// @todo: why it can't be done in constructor?
	mTwoDModel->createModelWidget();

	QObject::connect(mTwoDModelActionInfo.action(), &QAction::triggered
			, mTwoDModel.data(), &twoDModel::D2RobotModel::showModelWidget);
}

TwoDModelEngineFacade::~TwoDModelEngineFacade()
{
	delete mTwoDModelActionInfo.action();
}

qReal::ActionInfo &TwoDModelEngineFacade::showTwoDModelWidgetActionInfo()
{
	return mTwoDModelActionInfo;
}

interpreterBase::SensorsConfigurationProvider &TwoDModelEngineFacade::sensorsConfigurationProvider()
{
	return *mTwoDModel;
}

TwoDModelEngineInterface &TwoDModelEngineFacade::engine()
{
	return *mTwoDModel;
}
