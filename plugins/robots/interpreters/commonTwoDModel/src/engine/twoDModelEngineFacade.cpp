#include "commonTwoDModel/engine/twoDModelEngineFacade.h"

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

void TwoDModelEngineFacade::init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
		, qReal::SystemEvents const &systemEvents
		, qReal::GraphicalModelAssistInterface &graphicalModel
		, qReal::LogicalModelAssistInterface &logicalModel
		, qReal::gui::MainWindowInterpretersInterface const &interpretersInterface
		, interpreterBase::InterpreterControlInterface &interpreterControl)
{
	auto onActiveTabChanged = [this, &graphicalModel, &logicalModel] (qReal::Id const &id)
	{
		mView->setEnabled(!id.isNull());
		qReal::Id const logicalId = graphicalModel.logicalId(id);
		QString const xml = logicalId.isNull()
				? QString()
				: logicalModel.propertyByRoleName(logicalId, "worldModel").toString();
		QDomDocument worldModel;
		worldModel.setContent(xml);
		mView->loadXml(worldModel);
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
			, &interpreterControl, &interpretersInterface] (QDomDocument const &xml) {
				qReal::Id const logicalId = graphicalModel.logicalId(interpretersInterface.activeDiagram());
				if (!logicalId.isNull() && logicalId != qReal::Id::rootId()) {
					logicalModel.setPropertyByRoleName(logicalId, xml.toString(4), "worldModel");
				}
	});

	connect(&systemEvents, &qReal::SystemEvents::closedMainWindow, [=](){ mView.reset(); });

	connect(&eventsForKitPlugin
			, &interpreterBase::EventsForKitPluginInterface::robotModelChanged
			, [this, connectTwoDModel, disconnectTwoDModel](QString const &modelName) {
				bool const isCurrentModel = modelName == mRobotModelName;
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
