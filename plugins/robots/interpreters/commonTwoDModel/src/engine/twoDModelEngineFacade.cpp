#include "commonTwoDModel/engine/twoDModelEngineFacade.h"

#include "src/engine/d2RobotModel.h"

using namespace twoDModel::engine;

TwoDModelEngineFacade::TwoDModelEngineFacade(interpreterBase::robotModel::RobotModelInterface &robotModel
	, Configurer const * const configurer)
	: mRobotModelName(robotModel.name())
	, mTwoDModelActionInfo(
			new QAction(QIcon(":/icons/2d-model.svg"), QObject::tr("2d model"), nullptr)
			, "interpreters"
			, "tools")
	, mTwoDModel(new D2RobotModel(robotModel, configurer))
{
	connect(mTwoDModelActionInfo.action(), &QAction::triggered
			, mTwoDModel.data(), &twoDModel::D2RobotModel::showModelWidget);

	connect(mTwoDModel.data(), &D2RobotModel::runButtonPressed, this, &TwoDModelEngineFacade::runButtonPressed);
	connect(mTwoDModel.data(), &D2RobotModel::stopButtonPressed, this, &TwoDModelEngineFacade::stopButtonPressed);
	connect(mTwoDModel.data(), &D2RobotModel::widgetClosed, this, &TwoDModelEngineFacade::stopButtonPressed);
}

TwoDModelEngineFacade::~TwoDModelEngineFacade()
{
	delete mTwoDModelActionInfo.action();
}

void TwoDModelEngineFacade::init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
		, qReal::SystemEventsInterface const &systemEvents
		, interpreterBase::InterpreterControlInterface &interpreterControl)
{
	auto connectTwoDModel = [this, &eventsForKitPlugin, &interpreterControl]()
	{
		connect(&eventsForKitPlugin, &interpreterBase::EventsForKitPluginInterface::interpretationStarted
				, this, &twoDModel::TwoDModelControlInterface::onStartInterpretation
				, Qt::UniqueConnection);

		connect(&eventsForKitPlugin, &interpreterBase::EventsForKitPluginInterface::interpretationStopped
				, this, &twoDModel::TwoDModelControlInterface::onStopInterpretation
				, Qt::UniqueConnection);

		connect(this, &twoDModel::TwoDModelControlInterface::runButtonPressed
				, &interpreterControl, &interpreterBase::InterpreterControlInterface::interpret
				, Qt::UniqueConnection);

		connect(this, &twoDModel::TwoDModelControlInterface::stopButtonPressed
				, &interpreterControl, &interpreterBase::InterpreterControlInterface::stopRobot
				, Qt::UniqueConnection);
	};

	auto disconnectTwoDModel = [this, &eventsForKitPlugin, &interpreterControl]()
	{
		disconnect(&eventsForKitPlugin, &interpreterBase::EventsForKitPluginInterface::interpretationStarted
				, this, &twoDModel::TwoDModelControlInterface::onStartInterpretation);

		disconnect(&eventsForKitPlugin, &interpreterBase::EventsForKitPluginInterface::interpretationStopped
				, this, &twoDModel::TwoDModelControlInterface::onStopInterpretation);

		disconnect(this, &twoDModel::TwoDModelControlInterface::runButtonPressed
				, &interpreterControl, &interpreterBase::InterpreterControlInterface::interpret);

		disconnect(this, &twoDModel::TwoDModelControlInterface::stopButtonPressed
				, &interpreterControl, &interpreterBase::InterpreterControlInterface::stopRobot);
	};

	connect(&systemEvents, &qReal::SystemEventsInterface::closedMainWindow
			, mTwoDModel.data(), &twoDModel::D2RobotModel::closeModelWidget);

	connect(&eventsForKitPlugin
			, &interpreterBase::EventsForKitPluginInterface::robotModelChanged
			, [this, connectTwoDModel, disconnectTwoDModel](QString const &modelName) {
				bool const isCurrentModel = modelName == mRobotModelName;
				showTwoDModelWidgetActionInfo().action()->setVisible(isCurrentModel);
				if (isCurrentModel) {
					connectTwoDModel();
				} else {
					disconnectTwoDModel();
					mTwoDModel->closeModelWidget();
				}
			}
			);

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
