#include "commonTwoDModel/engine/twoDModelEngineFacade.h"

#include "src/engine/d2RobotModel.h"

using namespace twoDModel::engine;

TwoDModelEngineFacade::TwoDModelEngineFacade(interpreterBase::robotModel::RobotModelInterface &robotModel
	, Configurer const * const configurer)
	: mTwoDModelActionInfo(
			new QAction(QIcon(":/icons/2d-model.svg"), QObject::tr("2d model"), nullptr)
			, "interpreters"
			, "tools")
	, mTwoDModel(new D2RobotModel(robotModel, configurer))
{
	connect(mTwoDModelActionInfo.action(), &QAction::triggered
			, mTwoDModel.data(), &twoDModel::D2RobotModel::showModelWidget);

	connect(mTwoDModel.data(), &D2RobotModel::runButtonPressed, this, &TwoDModelEngineFacade::runButtonPressed);
	connect(mTwoDModel.data(), &D2RobotModel::stopButtonPressed, this, &TwoDModelEngineFacade::stopButtonPressed);
}

TwoDModelEngineFacade::~TwoDModelEngineFacade()
{
	delete mTwoDModelActionInfo.action();
}

void TwoDModelEngineFacade::init()
{
	mTwoDModel->init();
}

qReal::ActionInfo &TwoDModelEngineFacade::showTwoDModelWidgetActionInfo()
{
	return mTwoDModelActionInfo;
}

interpreterBase::DevicesConfigurationProvider &TwoDModelEngineFacade::devicesConfigurationProvider()
{
	return *mTwoDModel;
}

TwoDModelEngineInterface &TwoDModelEngineFacade::engine()
{
	return *mTwoDModel;
}

void TwoDModelEngineFacade::onStartInterpretation()
{
	mTwoDModel->startInterpretation();
}

void TwoDModelEngineFacade::onStopInterpretation()
{
	mTwoDModel->stopRobot();
}
