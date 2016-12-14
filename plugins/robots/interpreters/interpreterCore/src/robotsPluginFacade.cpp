/* Copyright 2013-2016 CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "interpreterCore/robotsPluginFacade.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/platformInfo.h>
#include <qrutils/widgets/consoleDock.h>
#include <kitBase/robotModel/portInfo.h>
#include <twoDModel/engine/twoDModelEngineInterface.h>
#include <twoDModel/engine/twoDModelGuiFacade.h>
#include <twoDModel/robotModel/twoDRobotModel.h>

#include <qrgui/textEditor/qscintillaTextEdit.h>

#include "interpreterCore/managers/kitAutoSwitcher.h"
#include "interpreterCore/interpreter/blockInterpreter.h"
#include "src/coreBlocks/coreBlocksFactory.h"
#include "src/ui/robotsSettingsPage.h"
#include "src/managers/exerciseExportManager.h"
#include "src/managers/uiManager.h"

using namespace interpreterCore;

RobotsPluginFacade::RobotsPluginFacade()
	: mKitPluginManager(qReal::PlatformInfo::invariantSettingsPath("pathToToolPlugins") + "/kitPlugins")
	, mActionsManager(mKitPluginManager, mRobotModelManager)
	, mDockDevicesConfigurer(nullptr)
	, mGraphicsWatcherManager(nullptr)
{
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, &mActionsManager, &ActionsManager::onRobotModelChanged);
}

RobotsPluginFacade::~RobotsPluginFacade()
{
	qDeleteAll(mInterpreters.values().toSet());
	mInterpreters.clear();
}

void RobotsPluginFacade::init(const qReal::PluginConfigurator &configurer)
{
	mActionsManager.init(&configurer.mainWindowInterpretersInterface());

	mRobotSettingsPage = new ui::RobotsSettingsPage(mKitPluginManager, mRobotModelManager
			, configurer.logicalModelApi());

	connect(&configurer.systemEvents(), &qReal::SystemEvents::activeTabChanged
			, mRobotSettingsPage, &ui::RobotsSettingsPage::onProjectOpened);

	mDevicesConfigurationManager.reset(new DevicesConfigurationManager(
			configurer.graphicalModelApi()
			, configurer.logicalModelApi()
			, configurer.mainWindowInterpretersInterface()
			, configurer.projectManager()
			));

	if (!selectKit()) {
		/// @todo Correctly handle unselected kit.
		return;
	}

	mMainWindow = &configurer.mainWindowInterpretersInterface();

	mParser.reset(new textLanguage::RobotsBlockParser(mRobotModelManager
			, [this]() { return mProxyInterpreter.timeElapsed(); }));

	kitBase::blocksBase::BlocksFactoryInterface * const coreFactory = new coreBlocks::CoreBlocksFactory();
	coreFactory->configure(configurer.graphicalModelApi()
			, configurer.logicalModelApi()
			, mRobotModelManager
			, *configurer.mainWindowInterpretersInterface().errorReporter()
			, *mParser
			);

	mBlocksFactoryManager.addFactory(coreFactory);

	mUiManager.reset(new UiManager(mActionsManager.debugModeAction()
			, mActionsManager.editModeAction()
			, configurer.mainWindowDockInterface()
			, configurer.systemEvents()
			, mEventsForKitPlugin
			, mRobotModelManager));

	interpreter::BlockInterpreter *interpreter = new interpreter::BlockInterpreter(
			configurer.graphicalModelApi()
			, configurer.logicalModelApi()
			, configurer.mainWindowInterpretersInterface()
			, configurer.projectManager()
			, mBlocksFactoryManager
			, mRobotModelManager
			, *mParser
			);

	registerInterpreter(interpreter);

	connect(&configurer.systemEvents(), &qReal::SystemEvents::closedMainWindow
			, &mProxyInterpreter, &kitBase::InterpreterInterface::userStopRobot);
	connect(&mProxyInterpreter, &kitBase::InterpreterInterface::connected
			, &mActionsManager.connectToRobotAction(), &QAction::setChecked);
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, &mProxyInterpreter, &kitBase::InterpreterInterface::userStopRobot);
	auto connectDisconnection = [this](kitBase::robotModel::RobotModelInterface &model) {
		connect(&model, &kitBase::robotModel::RobotModelInterface::disconnected
				, &mProxyInterpreter, &kitBase::InterpreterInterface::userStopRobot);
		connect(&model, &kitBase::robotModel::RobotModelInterface::disconnected
				, &mActionsManager, [=](){ mActionsManager.connectToRobotAction().setChecked(false); });
	};
	connectDisconnection(mRobotModelManager.model());
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged, &mProxyInterpreter, connectDisconnection);
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, &mProxyInterpreter, &kitBase::InterpreterInterface::userStopRobot);

	initKitPlugins(configurer);

	initSensorWidgets();

	mPaletteUpdateManager = new PaletteUpdateManager(configurer.mainWindowInterpretersInterface()
			, mBlocksFactoryManager, this);
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, mPaletteUpdateManager, &PaletteUpdateManager::updatePalette);

	// It will subscribe to all signals itself and free memory too.
	new KitAutoSwitcher(configurer.projectManager(), configurer.logicalModelApi()
			, mBlocksFactoryManager, mKitPluginManager, mRobotModelManager, this);

	mSaveAsTaskManager.reset(new ExerciseExportManager(configurer.logicalModelApi()
			, configurer.repoControlInterface(), configurer.projectManager()));

	connectInterpreterToActions();

	connectEventsForKitPlugin();

	connect(&mActionsManager.robotSettingsAction(), &QAction::triggered
			, [=] () { configurer.mainWindowInterpretersInterface().openSettingsDialog(tr("Robots")); });

	connect(&configurer.systemEvents(), &qReal::SystemEvents::activeTabChanged
			, &mActionsManager, &ActionsManager::onActiveTabChanged);

	// Just to capture them, not configurer.
	qReal::ProjectManagementInterface &projectManager = configurer.projectManager();
	qReal::GraphicalModelAssistInterface &graphicalModel = configurer.graphicalModelApi();
	connect(&mActionsManager.homeAction(), &QAction::triggered, [&projectManager, &graphicalModel, this]() {
		if (projectManager.somethingOpened()) {
			for (const qReal::Id &diagram : graphicalModel.children(qReal::Id::rootId())) {
				if (diagram.type() == qReal::Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode")) {
					mMainWindow->activateItemOrDiagram(diagram);
					return;
				}
			}
		} else {
			mMainWindow->openStartTab();
		}
	});

	const qrRepo::LogicalRepoApi &repoApi = configurer.logicalModelApi().logicalRepoApi();

	connect(&configurer.systemEvents(), &qReal::SystemEvents::activeTabChanged, this
			, [this, &repoApi] (const qReal::TabInfo &info) {
				Q_UNUSED(info);

				mDockDevicesConfigurer->setEnabled(
						!repoApi.metaInformation("twoDModelSensorsReadOnly").toBool());

				const bool hasReadOnlyFlags = repoApi.metaInformation("twoDModelWorldReadOnly").toBool()
						| repoApi.metaInformation("twoDModelSensorsReadOnly").toBool()
						| repoApi.metaInformation("twoDModelRobotPositionReadOnly").toBool()
						| repoApi.metaInformation("twoDModelRobotConfigurationReadOnly").toBool()
						| repoApi.metaInformation("twoDModelSimulationSettingsReadOnly").toBool()
						;

				mActionsManager.exportExerciseAction().setEnabled(!hasReadOnlyFlags);

				if (info.type() == qReal::TabInfo::TabType::editor) {
					mProxyInterpreter.resetInterpreter(mInterpreters[info.rootDiagramId().type()]);
				}
			});

	connect(&mActionsManager.exportExerciseAction(), &QAction::triggered
			, [this] () { mSaveAsTaskManager->save(); });

	mLogicalModelApi = &configurer.logicalModelApi();
	mTextManager = &configurer.textManager();
	mProjectManager = &configurer.projectManager();
	connect(mProjectManager
	        , &qReal::ProjectManagementInterface::afterOpen
	        , [&](const QString &path){
		auto logicalRepo = &mLogicalModelApi->logicalRepoApi();
		QString code = logicalRepo->metaInformation("activeCode").toString();
		QString name = logicalRepo->metaInformation("activeCodeName").toString();
		if (code.isEmpty() || name.isEmpty() || path.isEmpty()) {
			return;
		}
		QFileInfo codeDir(path);
		QFileInfo codePath(codeDir.dir().absoluteFilePath(name + ".js")); // absoluteDir?
		QFile codeFile(codePath.filePath());
		if (!codeFile.open(QFile::WriteOnly | QFile::Truncate)) {
			return;
		} // todo: check the result bool
		QTextStream(&codeFile) << code;
		codeFile.close();
		mTextManager->showInTextEditor(codePath, qReal::text::Languages::pickByExtension(codePath.suffix()));
	});

	sync();
}

qReal::gui::PreferencesPage *RobotsPluginFacade::robotsSettingsPage() const
{
	return mRobotSettingsPage;
}

interpreterCore::Customizer &RobotsPluginFacade::customizer()
{
	return mCustomizer;
}

ActionsManager &RobotsPluginFacade::actionsManager()
{
	return mActionsManager;
}

QObject *RobotsPluginFacade::guiScriptFacade() const
{
	const auto robotModel = dynamic_cast<twoDModel::robotModel::TwoDRobotModel *>(&mRobotModelManager.model());
	if (robotModel) {
		return &robotModel->engine()->guiFacade();
	}

	return nullptr;
}

QStringList RobotsPluginFacade::defaultSettingsFiles() const
{
	QStringList result = { ":/interpreterCoreDefaultSettings.ini" };
	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (kitBase::KitPluginInterface * const kit : mKitPluginManager.kitsById(kitId)) {
			const QString defaultSettings = kit->defaultSettingsFile();
			if (!defaultSettings.isEmpty()) {
				result << defaultSettings;
			}
		}
	}

	return result;
}

kitBase::InterpreterInterface &RobotsPluginFacade::interpreter()
{
	return mProxyInterpreter;
}

const kitBase::InterpreterInterface &RobotsPluginFacade::interpreter() const
{
	return mProxyInterpreter;
}

bool RobotsPluginFacade::interpretCode(const QString &inputs)
{
	auto logicalRepo = &mLogicalModelApi->logicalRepoApi();
	QString code = logicalRepo->metaInformation("activeCode").toString();
	QString name = logicalRepo->metaInformation("activeCodeName").toString();//not needed?
	if (code.isEmpty() || name.isEmpty()) {
		qDebug("No saved js code found in the qrs file");
		return false;
	}
	emit mEventsForKitPlugin.interpretCode(code, inputs);
	return true;
}

void RobotsPluginFacade::saveCode(const QString &code)
{
	auto logicalRepo = &mLogicalModelApi->mutableLogicalRepoApi();
	logicalRepo->setMetaInformation("activeCode", code);
	logicalRepo->setMetaInformation("activeCodeName", "script");// no concise name for now
	mProjectManager->setUnsavedIndicator(true);
}

void RobotsPluginFacade::openSavedCode()
{
	auto logicalRepo = &mLogicalModelApi->mutableLogicalRepoApi();
	QString code = logicalRepo->metaInformation("activeCode").toString();
	// probably this method is to be deleted later
}

void RobotsPluginFacade::connectInterpreterToActions()
{
	QObject::connect(
			&mActionsManager.runAction()
			, &QAction::triggered
			, &mProxyInterpreter
			, &kitBase::InterpreterInterface::interpret
			);

	QObject::connect(
			&mActionsManager.stopRobotAction()
			, &QAction::triggered
			, &mProxyInterpreter
			, &kitBase::InterpreterInterface::userStopRobot
			);

	QObject::connect(
			&mActionsManager.connectToRobotAction()
			, &QAction::triggered
			, &mProxyInterpreter
			, &kitBase::InterpreterInterface::connectToRobot
			);
}

bool RobotsPluginFacade::selectKit()
{
	/// @todo reinit it each time when robot model changes
	/// @todo: do we need this method?
	const QString selectedKit = qReal::SettingsManager::value("SelectedRobotKit").toString();
	if (selectedKit.isEmpty() && !mKitPluginManager.kitIds().isEmpty()) {
		qReal::SettingsManager::setValue("SelectedRobotKit", mKitPluginManager.kitIds()[0]);
	} else if (mKitPluginManager.kitIds().isEmpty()) {
		mPaletteUpdateManager->disableAll();

		/// @todo Correctly handle unselected kit.
		return false;
	}

	return true;
}

void RobotsPluginFacade::initSensorWidgets()
{
	mDockDevicesConfigurer.reset(new kitBase::DevicesConfigurationWidget(nullptr, true));
	mDockDevicesConfigurer->loadRobotModels(mKitPluginManager.allRobotModels());
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, mDockDevicesConfigurer.data(), &kitBase::DevicesConfigurationWidget::selectRobotModel);

	mWatchListWindow = new utils::WatchListWindow(*mParser);

	auto hideVariables = [=]() { mWatchListWindow->hideVariables(mParser->hiddenVariables()); };
	hideVariables();
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged, hideVariables);

	mGraphicsWatcherManager = new GraphicsWatcherManager(*mParser, mRobotModelManager, this);
	connect(&mProxyInterpreter, &kitBase::InterpreterInterface::started
			, mGraphicsWatcherManager, &GraphicsWatcherManager::forceStart);
	connect(&mProxyInterpreter, &kitBase::InterpreterInterface::stopped
			, mGraphicsWatcherManager, &GraphicsWatcherManager::forceStop);
	connect(&mProxyInterpreter, &kitBase::InterpreterInterface::started, mGraphicsWatcherManager, [=]() {
		mActionsManager.runAction().setVisible(false);
		mActionsManager.stopRobotAction().setVisible(mRobotModelManager.model().interpretedModel());
	});
	connect(&mProxyInterpreter
			, &kitBase::InterpreterInterface::stopped
			, mGraphicsWatcherManager
			, [=] () {
		if (!dynamic_cast<qReal::text::QScintillaTextEdit *>(mMainWindow->currentTab())) {
			// since userStop fires on any tab/model switch even when the code tab is opened
			// and nothing is running, but this whole visibility mumbo-jumbo has become a mess
			mActionsManager.runAction().setVisible(mRobotModelManager.model().interpretedModel());
			mActionsManager.stopRobotAction().setVisible(false);
		}
	});

	mUiManager->placeDevicesConfig(mDockDevicesConfigurer.data());
	mUiManager->placeWatchPlugins(mWatchListWindow, mGraphicsWatcherManager->widget());
	mActionsManager.appendHotKey("View.ToggleRobotConsole", tr("Toggle robot console panel")
			, *mUiManager->robotConsole().toggleViewAction());
	for (kitBase::robotModel::RobotModelInterface * const model : mKitPluginManager.allRobotModels()) {
		for (kitBase::KitPluginInterface * const kit : mKitPluginManager.kitsById(model->kitId())) {
			mUiManager->addWidgetToToolbar(*model, kit->quickPreferencesFor(*model));
		}
	}

	mDevicesConfigurationManager->connectDevicesConfigurationProvider(mRobotSettingsPage);
	mDevicesConfigurationManager->connectDevicesConfigurationProvider(mDockDevicesConfigurer.data());
	mDevicesConfigurationManager->connectDevicesConfigurationProvider(mGraphicsWatcherManager);
}

void RobotsPluginFacade::initKitPlugins(const qReal::PluginConfigurator &configurer)
{
	/// @todo: Check that this code works when different kit is selected
	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (kitBase::KitPluginInterface * const kit : mKitPluginManager.kitsById(kitId)) {
			kit->init(kitBase::KitPluginConfigurator(configurer
					, mRobotModelManager, *mParser, mEventsForKitPlugin, mProxyInterpreter));

			for (const kitBase::robotModel::RobotModelInterface *model : kit->robotModels()) {
				initFactoriesFor(kitId, model, configurer);
				connect(&mEventsForKitPlugin, &kitBase::EventsForKitPluginInterface::interpretationStarted
						, model, &kitBase::robotModel::RobotModelInterface::onInterpretationStarted);
			}

			mDevicesConfigurationManager->connectDevicesConfigurationProvider(kit->devicesConfigurationProvider());
			for (kitBase::InterpreterInterface * const interpreter : kit->customInterpreters()) {
				registerInterpreter(interpreter);
			}
		}
	}
}

void RobotsPluginFacade::initFactoriesFor(const QString &kitId
		, const kitBase::robotModel::RobotModelInterface *model
		, const qReal::PluginConfigurator &configurer)
{
	// Pulling each robot model to each kit plugin with same ids. We need it for supporting
	// plugin-based blocks set extension for concrete roobt model.
	for (kitBase::KitPluginInterface * const kit : mKitPluginManager.kitsById(kitId)) {
		kitBase::blocksBase::BlocksFactoryInterface * const factory = kit->blocksFactoryFor(model);
		if (factory) {
			/// @todo Non-obvious dependency on mParser, which may or may not be constructed here.
			///       More functional style will be helpful here.
			factory->configure(configurer.graphicalModelApi()
					, configurer.logicalModelApi()
					, mRobotModelManager
					, *configurer.mainWindowInterpretersInterface().errorReporter()
					, *mParser
					);

			mBlocksFactoryManager.addFactory(factory, model);
		}
	}
}

void RobotsPluginFacade::connectEventsForKitPlugin()
{
	QObject::connect(
			&mProxyInterpreter
			, &kitBase::InterpreterInterface::started
			, &mEventsForKitPlugin
			, &kitBase::EventsForKitPluginInterface::interpretationStarted
			);

	QObject::connect(
			&mProxyInterpreter
			, &kitBase::InterpreterInterface::stopped
			, &mEventsForKitPlugin
			, &kitBase::EventsForKitPluginInterface::interpretationStopped
			);

	QObject::connect(
				&mEventsForKitPlugin
				, &kitBase::EventsForKitPluginInterface::interpretationStarted
				, [this](){ /// @todo
		const bool isBlockInt = mProxyInterpreter.isRunning();
		mActionsManager.runAction().setEnabled(isBlockInt);
		mActionsManager.stopRobotAction().setEnabled(isBlockInt);
		mActionsManager.setEnableRobotActions(isBlockInt);
	}
	);

	QObject::connect(
				&mEventsForKitPlugin
				, &kitBase::EventsForKitPluginInterface::codeInterpretationStarted
				, this
				, &RobotsPluginFacade::saveCode
				);

	QObject::connect(
				&mEventsForKitPlugin
				, &kitBase::EventsForKitPluginInterface::interpretationStopped
				, [this](qReal::interpretation::StopReason reason){ /// @todo
		Q_UNUSED(reason);
		mActionsManager.runAction().setEnabled(true);
		mActionsManager.stopRobotAction().setEnabled(true);
		mActionsManager.setEnableRobotActions(true);
	}
	);

	QObject::connect(
			&mRobotModelManager
			, &RobotModelManager::robotModelChanged
			, [this](kitBase::robotModel::RobotModelInterface &model) {
				emit mEventsForKitPlugin.robotModelChanged(model.name());
			}
	);
}

void RobotsPluginFacade::registerInterpreter(kitBase::InterpreterInterface * const interpreter)
{
	bool allDiagramsAlreadyRegistered = true;
	const qReal::IdList diagrams = interpreter->supportedDiagrams();
	for (const qReal::Id &diagram : diagrams) {
		if (!mInterpreters.contains(diagram)) {
			allDiagramsAlreadyRegistered = false;
			mInterpreters[diagram] = interpreter;
		}
	}

	if (allDiagramsAlreadyRegistered) {
		delete interpreter;
	} else if (auto provider = dynamic_cast<kitBase::DevicesConfigurationProvider *>(interpreter)) {
		mDevicesConfigurationManager->connectDevicesConfigurationProvider(provider);
	}
}

void RobotsPluginFacade::sync()
{
	mRobotModelManager.sync();
}
