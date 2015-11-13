/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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

#include "uiManager.h"

#include <QtCore/QTimer>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>

#include <qrkernel/logging.h>
#include <qrkernel/settingsManager.h>
#include <qrutils/inFile.h>
#include <qrutils/smartDock.h>
#include <qrutils/widgets/consoleDock.h>
#include <kitBase/robotModel/robotModelUtils.h>
#include <kitBase/robotModel/robotParts/shell.h>

#include "src/ui/modeStripe.h"

const int lowerMediumResolutionBorder = 1024;
const int upperMediumResolutionBorder = 1280;

using namespace interpreterCore;

static const QColor backgrondColor = QPalette().color(QPalette::Background);
static const QColor editModeColor = QPalette().color(QPalette::Background);
static const QColor debugModeColor = qRgb(152, 251, 152);

UiManager::UiManager(QAction &debugModeAction
		, QAction &editModeAction
		, qReal::gui::MainWindowDockInterface &mainWindow
		, qReal::SystemEvents &systemEvents
		, kitBase::EventsForKitPluginInterface &kitPluginEvents
		, kitBase::robotModel::RobotModelManagerInterface &robotModelManager)
	: mDebugModeAction(debugModeAction)
	, mEditModeAction(editModeAction)
	, mMainWindow(mainWindow)
	, mTabBar(nullptr)
	, mCustomWidgetsBar(new QToolBar(tr("Miscellaneous"), mMainWindow.windowWidget()))
	, mRobotConsole(new qReal::ui::ConsoleDock(tr("Robot console"), mMainWindow.windowWidget()))
{
	mMainWindow.graphicalModelDock()->setWindowTitle(QObject::tr("Blocks"));

	connect(&systemEvents, &qReal::SystemEvents::activeTabChanged, this, &UiManager::onActiveTabChanged);
	connect(&systemEvents, &qReal::SystemEvents::ensureDiagramVisible, this, &UiManager::ensureDiagramVisible);
	connect(&kitPluginEvents, &kitBase::EventsForKitPluginInterface::interpretationStarted
			, this, &UiManager::switchToDebuggerMode);
	connect(&kitPluginEvents, &kitBase::EventsForKitPluginInterface::interpretationStarted
			, mRobotConsole, &qReal::ui::ConsoleDock::clear);
	connect(&kitPluginEvents, &kitBase::EventsForKitPluginInterface::robotModelChanged
			, [=]() { QTimer::singleShot(0, this, SLOT(reloadDocksSavingToolbarsAndErrors())); });
	connect(&robotModelManager, &kitBase::robotModel::RobotModelManagerInterface::robotModelChanged
			, this, &UiManager::onRobotModelChanged);
	connect(&debugModeAction, &QAction::triggered, this, &UiManager::switchToDebuggerMode);
	connect(&editModeAction, &QAction::triggered, this, &UiManager::switchToEditorMode);

	mRobotConsole->hide();
	initTab();
	mCustomWidgetsBar->setObjectName("robotsMiscellaneousBar");
	connect(this, &QObject::destroyed, [this]() { mCustomWidgetsBar->setParent(nullptr); });
	mMainWindow.addToolBar(Qt::TopToolBarArea, mCustomWidgetsBar);
	mMainWindow.addDockWidget(Qt::BottomDockWidgetArea, mRobotConsole);
	mMainWindow.tabifyDockWidget(mRobotConsole, mMainWindow.errorReporterDock());
	mMainWindow.windowWidget()->addAction(mRobotConsole->toggleViewAction());
	mRobotConsole->toggleViewAction()->setShortcut(Qt::ALT + Qt::Key_2);

	mMainWindow.statusBar()->setAutoFillBackground(true);
#ifdef Q_OS_LINUX
	// On Windows and Mac this stylesheet is not needed and will cause segfault on program close.
	mMainWindow.statusBar()->setStyleSheet("QStatusBar::item { border: 0px solid black; padding: 10px; }");
#endif
	editModeAction.setProperty("modeName", tr("edit mode"));
	debugModeAction.setProperty("modeName", tr("debug mode"));
	produceModeButton(Mode::Editing, debugModeAction, mMainWindow.statusBar());
	produceModeButton(Mode::Debugging, editModeAction, mMainWindow.statusBar());

	switchToEditorMode();
	onActiveTabChanged(qReal::TabInfo());
}

void UiManager::placeDevicesConfig(QWidget *devicesWidget)
{
	QDockWidget * const devicesDock = produceDockWidget(QObject::tr("Configure devices"), devicesWidget);
	devicesDock->setObjectName("devicesConfigurationDock");
	connect(this, &QObject::destroyed, [devicesDock](){ devicesDock->setParent(nullptr); });
	mMainWindow.addDockWidget(Qt::LeftDockWidgetArea, devicesDock);
}

void UiManager::placeWatchPlugins(QDockWidget *watchWindow, QWidget *graphicsWatch)
{
	mMainWindow.addDockWidget(Qt::LeftDockWidgetArea, watchWindow);
	watchWindow->setObjectName("variablesDebuggerDock");
	watchWindow->setFloating(false);

	QDockWidget * const graphWatchDock = produceDockWidget(QObject::tr("Sensors state"), graphicsWatch);
	graphWatchDock->setObjectName("graphicsWatcherDock");
	mMainWindow.addDockWidget(Qt::LeftDockWidgetArea, graphWatchDock);

	connect(this, &QObject::destroyed, [watchWindow](){ watchWindow->setParent(nullptr); });
	connect(this, &QObject::destroyed, [graphWatchDock](){ graphWatchDock->setParent(nullptr); });

	mMainWindow.tabifyDockWidget(watchWindow, graphWatchDock);
	reloadDocks();
}

void UiManager::addWidgetToToolbar(kitBase::robotModel::RobotModelInterface &robotModel, QWidget * const widget)
{
	if (!widget) {
		return;
	}

	// Toolbar will take ownership on widget and resulting action.
	QAction * const action = mCustomWidgetsBar->addWidget(widget);
	mToolBarWidgets[action] = &robotModel;

	connect(action, &QAction::changed, [this]() {
		for (QAction * const action : mCustomWidgetsBar->actions()) {
			if (action->isVisible()) {
				mCustomWidgetsBar->setVisible(true);
				return;
			}
		}

		mCustomWidgetsBar->hide();
	});
}

qReal::ui::ConsoleDock &UiManager::robotConsole()
{
	return *mRobotConsole;
}

void UiManager::onActiveTabChanged(const qReal::TabInfo &tab)
{
	if (tab.type() == mCurrentTab) {
		return;
	}

	saveDocks();
	mCurrentTab = tab.type();
	reloadDocks();
	toggleModeButtons();
}

void UiManager::onRobotModelChanged(kitBase::robotModel::RobotModelInterface &model)
{
	for (QAction * const action : mToolBarWidgets.keys()) {
		const kitBase::robotModel::RobotModelInterface *bindedModel = mToolBarWidgets[action];
		action->setVisible(!bindedModel || bindedModel == &model);
	}

	auto subscribeShell = [this, &model]() {
		if (kitBase::robotModel::robotParts::Shell * const shell = kitBase::robotModel::RobotModelUtils::findDevice
				<kitBase::robotModel::robotParts::Shell>(model, "ShellPort"))
		{
			connect(shell, &kitBase::robotModel::robotParts::Shell::textPrinted
					, mRobotConsole, &qReal::ui::ConsoleDock::print, Qt::UniqueConnection);
		}
	};

	// Shell can be already configured or not. However, checking for it now or later, when everything is ready for use.
	subscribeShell();
	connect(&model, &kitBase::robotModel::RobotModelInterface::allDevicesConfigured, subscribeShell);
}

void UiManager::switchToEditorMode()
{
	switchToMode(Mode::Editing);
}

void UiManager::switchToDebuggerMode()
{
	switchToMode(Mode::Debugging);
}

void UiManager::switchToMode(UiManager::Mode mode)
{
	if (mCurrentMode == mode) {
		return;
	}

	saveDocks();
	mCurrentMode = mode;
	reloadDocksSavingToolbarsAndErrors();
	toggleModeButtons();
}

void UiManager::toggleModeButtons()
{
	// First hiding status bar to prevent its expanding over the screen...
	mMainWindow.statusBar()->setVisible(false);

	// And just then setting actual visibility...
	mEditModeAction.setVisible(mCurrentTab != qReal::TabInfo::TabType::other);
	mDebugModeAction.setVisible(mCurrentTab != qReal::TabInfo::TabType::other);
	mEditModeAction.setChecked(mCurrentMode == Mode::Editing);
	mDebugModeAction.setChecked(mCurrentMode == Mode::Debugging);
	if (mTabBar) {
		mTabBar->setVisible(mCurrentTab != qReal::TabInfo::TabType::other);
	}

	const QColor color = mCurrentTab == qReal::TabInfo::TabType::other
			? backgrondColor
			: mCurrentMode == Mode::Editing ? editModeColor : debugModeColor;
	QPalette palette;
	palette.setColor(QPalette::Background, color);
	palette.setColor(QPalette::Base, color);
	mMainWindow.statusBar()->setPalette(palette);
	// And then showing status bar again
	mMainWindow.statusBar()->setVisible(true);
}

QDockWidget *UiManager::produceDockWidget(const QString &title, QWidget *content) const
{
	QDockWidget * const dock = new QDockWidget(title);
	dock->setWidget(content);
	return dock;
}

void UiManager::produceModeButton(UiManager::Mode mode, QAction &action, QStatusBar *statusBar) const
{
	QWidget *result = nullptr;
	switch (mode) {
	case Mode::Dummy:
		return;
	case Mode::Editing:
		result = new ui::ModeStripe(action, tr("Edit mode"), statusBar);
		break;
	case Mode::Debugging:
		result = new ui::ModeStripe(action, tr("Debug mode"), statusBar);
		break;
	}

	if (!result) {
		qWarning() << "Forgot to implement producing status bar button for mode" << static_cast<int>(mode);
		return;
	}

	result->setVisible(false);
	statusBar->addWidget(result, 10);
	connect(this, &QObject::destroyed, [result]() { result->setParent(nullptr); });
}

int UiManager::currentMode() const
{
	return static_cast<int>(mCurrentTab) | static_cast<int>(mCurrentMode);
}

QString UiManager::currentSettingsKey() const
{
	return "docksStateInMode" + QString::number(currentMode());
}

void UiManager::saveDocks() const
{
	qReal::SettingsManager::setValue(currentSettingsKey(), mMainWindow.saveState(currentMode()));
}

void UiManager::reloadDocks() const
{
	hack2dModelDock();
	const QByteArray state = qReal::SettingsManager::value(currentSettingsKey()).toByteArray();
	if (!mMainWindow.restoreState(state, currentMode())) {
		QLOG_ERROR() << "Cannot apply docks state for mode" << currentMode() << ":" << state;
	} else {
		resetMainWindowCorners();
		// Same trick as main window does with error reporter.
		if (mRobotConsole->isEmpty()) {
			mRobotConsole->hide();
		}
	}
}

void UiManager::reloadDocksSavingToolbarsAndErrors() const
{
	// To this moment toolbars already updated their visibility. Calling just reloadDocks() here
	// will loose some toolbars visibility and error reporter state, so memorizing it here...
	const bool errorReporterWasVisible = mMainWindow.errorReporterDock()->isVisible();
	const bool robotConsoleWasVisible = mRobotConsole->isVisible();
	QMap<QToolBar *, bool> toolBarsVisiblity;
	for (QToolBar * const toolBar : mMainWindow.toolBars()) {
		toolBarsVisiblity[toolBar] = toolBar->isVisible();
	}

	// Now reloading docks, toolbars are in random visibility after this...
	reloadDocks();

	// And finally restoring old configuration.
	mMainWindow.errorReporterDock()->setVisible(errorReporterWasVisible);
	mRobotConsole->setVisible(robotConsoleWasVisible);
	for (QToolBar * const toolBar : toolBarsVisiblity.keys()) {
		toolBar->setVisible(toolBarsVisiblity[toolBar]);
	}
}

void UiManager::resetMainWindowCorners() const
{
	// Seems like on different platforms the default corner occupation is different, so fixing it here...
	mMainWindow.setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
	mMainWindow.setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
	mMainWindow.setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
	mMainWindow.setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
}

void UiManager::ensureDiagramVisible()
{
	if (mCurrentMode == Mode::Editing) {
		return;
	}

	// 2D model is placed into smart dock that may hide central widget if docked into TopDockWidgetArea.
	// If we met such case then switching to editor mode.
	for (utils::SmartDock * const twoDModel : mMainWindow.windowWidget()->findChildren<utils::SmartDock *>()) {
		if (twoDModel->isCentral()) {
			switchToEditorMode();
			return;
		}
	}
}

void UiManager::initTab()
{
	connect(&mEditModeAction, &QAction::triggered, this, &UiManager::switchToEditorMode);
	connect(&mDebugModeAction, &QAction::triggered, this, &UiManager::switchToDebuggerMode);
	connect(&mEditModeAction, &QAction::toggled, this, &UiManager::toggleModeButtons);
	connect(&mDebugModeAction, &QAction::toggled, this, &UiManager::toggleModeButtons);

	const QSize resolution = QApplication::desktop()->screenGeometry().size();
	if (resolution.width() < lowerMediumResolutionBorder) {
		// The screen is super-small, do not show tab bar at all
		mMainWindow.statusBar()->addAction(&mEditModeAction);
		mMainWindow.statusBar()->addAction(&mDebugModeAction);
		return;
	}

	mTabBar = new QToolBar(tr("Modes"), mMainWindow.windowWidget());
	mTabBar->setObjectName("largeTabsBar");
	mTabBar->setIconSize(QSize(32, 32));
	mTabBar->setToolButtonStyle(resolution.width() < upperMediumResolutionBorder
			? Qt::ToolButtonIconOnly // On small resolutions in some locales text may be too wide.
			: Qt::ToolButtonTextUnderIcon);
	mMainWindow.addToolBar(Qt::LeftToolBarArea, mTabBar);
	mTabBar->addAction(&mEditModeAction);
	mTabBar->addAction(&mDebugModeAction);
}

void UiManager::hack2dModelDock() const
{
	// 2D model is placed into smart dock: it may be embedded into instance of QDialog
	// that is not influeced by mMainWindow::restoreState. So we must first switch to a docked form
	// and then restore docks state.
	if (utils::SmartDock * const twoDModel = mMainWindow.windowWidget()->findChild<utils::SmartDock *>()) {
		twoDModel->switchToDocked();
	}
}

void UiManager::enableDocksSnapshotter() const
{
	// This method provides tools only for development.
	// It must not be called in master branch code.
	QWidget * const mainWindow = dynamic_cast<QWidget *>(&mMainWindow);
	QDialog * const dialog = new QDialog(mainWindow);
	QVBoxLayout * const layout = new QVBoxLayout;
	dialog->setLayout(layout);
	QPushButton * const button = new QPushButton("Snapshot docks", mainWindow);
	QLineEdit * const lineEdit = new QLineEdit(mainWindow);
	connect(button, &QPushButton::clicked, [=]() {
		const QString tempSettingsFileName = "tempFileForStoringWindowState";
		QSettings tempSettings(tempSettingsFileName, QSettings::IniFormat);
		tempSettings.setValue(currentSettingsKey(), mMainWindow.saveState(currentMode()));
		tempSettings.sync();
		lineEdit->setText(utils::InFile::readAll(tempSettingsFileName).split("\n", QString::SkipEmptyParts).last());
		QFile::remove(tempSettingsFileName);
	});
	layout->addWidget(button);
	layout->addWidget(lineEdit);
	dialog->show();
}
