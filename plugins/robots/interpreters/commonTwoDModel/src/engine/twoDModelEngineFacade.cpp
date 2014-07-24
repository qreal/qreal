#include "commonTwoDModel/engine/twoDModelEngineFacade.h"

#include "model/model.h"
#include "view/d2ModelWidget.h"
#include "twoDModelEngineApi.h"

using namespace twoDModel::engine;

TwoDModelEngineFacade::TwoDModelEngineFacade(interpreterBase::robotModel::RobotModelInterface &robotModel
		, Configurer const * const configurer)
	: mRobotModelName(robotModel.name())
	, mTwoDModelActionInfo(
			new QAction(QIcon(":/icons/2d-model.svg"), QObject::tr("2d model"), nullptr)
			, "interpreters"
			, "tools")
	, mModel(new model::Model(robotModel, configurer))
	, mView(new view::D2ModelWidget(*mModel.data(), configurer))
	, mApi(new TwoDModelEngineApi(*mModel.data(), *mView.data(), configurer))
{
	connect(mTwoDModelActionInfo.action(), &QAction::triggered, mView.data(), &view::D2ModelWidget::init);

	connect(mView.data(), &view::D2ModelWidget::runButtonPressed, this, &TwoDModelEngineFacade::runButtonPressed);
	connect(mView.data(), &view::D2ModelWidget::stopButtonPressed, this, &TwoDModelEngineFacade::stopButtonPressed);
	connect(mView.data(), &view::D2ModelWidget::widgetClosed, this, &TwoDModelEngineFacade::stopButtonPressed);
}

TwoDModelEngineFacade::~TwoDModelEngineFacade()
{
	delete mTwoDModelActionInfo.action();
}

void TwoDModelEngineFacade::init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
		, qReal::SystemEventsInterface const &systemEvents
		, qReal::GraphicalModelAssistInterface &graphicalModel
		, qReal::LogicalModelAssistInterface &logicalModel
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

	connect(&systemEvents, &qReal::SystemEventsInterface::activeTabChanged
			, [this, &graphicalModel, &logicalModel] (qReal::Id const &id) {
				mView->setRunStopButtonsEnabled(!id.isNull());
				qReal::Id const logicalId = graphicalModel.logicalId(id);
				mActiveDiagramLogicalId = logicalId;
				QString const xml = logicalId.isNull()
						? QString()
						: logicalModel.propertyByRoleName(logicalId, "worldModel").toString();
				QDomDocument worldModel;
				worldModel.setContent(xml);
				mView->loadXml(worldModel);
	});

	connect(mModel.data(), &model::Model::modelChanged,
			[this, &logicalModel, &interpreterControl] (QDomDocument const &xml) {
				qReal::Id const logicalId = mActiveDiagramLogicalId;
				if (!logicalId.isNull() && logicalId != qReal::Id::rootId()) {
					logicalModel.setPropertyByRoleName(logicalId, xml.toString(4), "worldModel");
				}
	});

	connect(&systemEvents, &qReal::SystemEventsInterface::closedMainWindow
			, [=](){ mView.reset(); });

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
