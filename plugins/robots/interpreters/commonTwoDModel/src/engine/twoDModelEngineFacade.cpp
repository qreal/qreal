#include "commonTwoDModel/engine/twoDModelEngineFacade.h"

#include <interpreterBase/readOnly.h>

#include "commonTwoDModel/engine/view/d2ModelWidget.h"
#include "model/model.h"
#include "twoDModelEngineApi.h"

using namespace twoDModel::engine;

TwoDModelEngineFacade::TwoDModelEngineFacade(twoDModel::robotModel::TwoDRobotModel &robotModel)
	: mRobotModelName(robotModel.name())
	, mTwoDModelActionInfo(
			new QAction(QIcon(":/icons/2d-model.svg"), QObject::tr("2d model"), nullptr)
			, "interpreters"
			, "tools")
	, mModel(new model::Model())
	, mView(new view::D2ModelWidget(*mModel.data()))
	, mApi(new TwoDModelEngineApi(*mModel.data(), *mView.data()))
{
	mModel.data()->addRobotModel(robotModel);
	connect(mTwoDModelActionInfo.action(), &QAction::triggered, mView.data(), &view::D2ModelWidget::init);

	connect(mView.data(), &view::D2ModelWidget::runButtonPressed, this, &TwoDModelEngineFacade::runButtonPressed);
	connect(mView.data(), &view::D2ModelWidget::stopButtonPressed, this, &TwoDModelEngineFacade::stopButtonPressed);
	connect(mView.data(), &view::D2ModelWidget::widgetClosed, this, &TwoDModelEngineFacade::stopButtonPressed);
}

TwoDModelEngineFacade::~TwoDModelEngineFacade()
{
}

void TwoDModelEngineFacade::init(const interpreterBase::EventsForKitPluginInterface &eventsForKitPlugin
		, const qReal::SystemEvents &systemEvents
		, qReal::GraphicalModelAssistInterface &graphicalModel
		, qReal::LogicalModelAssistInterface &logicalModel
		, const qReal::gui::MainWindowInterpretersInterface &interpretersInterface
		, interpreterBase::InterpreterControlInterface &interpreterControl)
{
	auto onActiveTabChanged = [this, &graphicalModel, &logicalModel] (const qReal::Id &id)
	{
		mView->setEnabled(!id.isNull());
		const qReal::Id logicalId = graphicalModel.logicalId(id);
		const QString xml = logicalId.isNull()
				? QString()
				: logicalModel.propertyByRoleName(logicalId, "worldModel").toString();
		QDomDocument worldModel;
		worldModel.setContent(xml);
		mView->loadXml(worldModel);

		loadReadOnlyFlags(logicalModel);
	};

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

	connect(&systemEvents, &qReal::SystemEvents::activeTabChanged, onActiveTabChanged);

	connect(mModel.data(), &model::Model::modelChanged, [this, &graphicalModel, &logicalModel
			, &interpreterControl, &interpretersInterface] (const QDomDocument &xml) {
				const qReal::Id logicalId = graphicalModel.logicalId(interpretersInterface.activeDiagram());
				if (!logicalId.isNull() && logicalId != qReal::Id::rootId()) {
					logicalModel.setPropertyByRoleName(logicalId, xml.toString(4), "worldModel");
				}
	});

	connect(&systemEvents, &qReal::SystemEvents::closedMainWindow, [=](){ mView.reset(); });

	connect(&eventsForKitPlugin
			, &interpreterBase::EventsForKitPluginInterface::robotModelChanged
			, [this, connectTwoDModel, disconnectTwoDModel](const QString &modelName) {
				const bool isCurrentModel = modelName == mRobotModelName;
				showTwoDModelWidgetActionInfo().action()->setVisible(isCurrentModel);
				if (isCurrentModel) {
					connectTwoDModel();
				} else {
					disconnectTwoDModel();
					mView->close();
				}
			}
			);
}

qReal::ActionInfo &TwoDModelEngineFacade::showTwoDModelWidgetActionInfo()
{
	return mTwoDModelActionInfo;
}

interpreterBase::DevicesConfigurationProvider &TwoDModelEngineFacade::devicesConfigurationProvider()
{
	return *mView.data();
}

TwoDModelEngineInterface &TwoDModelEngineFacade::engine()
{
	return *mApi.data();
}

void TwoDModelEngineFacade::onStartInterpretation()
{
	mModel->timeline().start();
}

void TwoDModelEngineFacade::onStopInterpretation()
{
	mModel->timeline().stop();
}

void TwoDModelEngineFacade::loadReadOnlyFlags(const qReal::LogicalModelAssistInterface &logicalModel)
{
	int readOnlyFlags = interpreterBase::ReadOnly::None;

	const auto load = [&] (const QString &tag, interpreterBase::ReadOnly::ReadOnlyEnum flag) {
		if (logicalModel.logicalRepoApi().metaInformation(tag).toBool()) {
			readOnlyFlags |= flag;
		}
	};

	load("twoDModelWorldReadOnly", interpreterBase::ReadOnly::World);
	load("twoDModelSensorsReadOnly", interpreterBase::ReadOnly::Sensors);
	load("twoDModelRobotPositionReadOnly", interpreterBase::ReadOnly::RobotPosition);
	load("twoDModelRobotConfigurationReadOnly", interpreterBase::ReadOnly::RobotSetup);
	load("twoDModelSimulationSettingsReadOnly", interpreterBase::ReadOnly::SimulationSettings);

	mView->setInteractivityFlags(readOnlyFlags);
}
