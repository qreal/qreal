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
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>

#include <qrkernel/logging.h>
#include <qrkernel/settingsManager.h>
#include <qrutils/inFile.h>

#include "src/ui/modeStripe.h"

using namespace interpreterCore;

static const QColor backgrondColor = QPalette().color(QPalette::Background);
static const QColor editModeColor = QPalette().color(QPalette::Background);
//static const QColor editModeColor = qRgb(135, 206, 250);
static const QColor debugModeColor = qRgb(152, 251, 152);

UiManager::UiManager(QAction &debugModeAction
		, QAction &editModeAction
		, qReal::gui::MainWindowDockInterface &mainWindow
		, qReal::SystemEvents &systemEvents
		, kitBase::EventsForKitPluginInterface &kitPluginEvents)
	: mDebugModeAction(debugModeAction)
	, mEditModeAction(editModeAction)
	, mMainWindow(mainWindow)
{
	mMainWindow.graphicalModelDock()->setWindowTitle(QObject::tr("Blocks"));

	connect(&systemEvents, &qReal::SystemEvents::activeTabChanged, this, &UiManager::onActiveTabChanged);
	connect(&kitPluginEvents, &kitBase::EventsForKitPluginInterface::interpretationStarted
			, this, &UiManager::switchToDebuggerMode);
	connect(&kitPluginEvents, &kitBase::EventsForKitPluginInterface::robotModelChanged
			, [=]() { QTimer::singleShot(0, this, SLOT(reloadDocks())); });
	connect(&debugModeAction, &QAction::triggered, this, &UiManager::switchToDebuggerMode);
	connect(&editModeAction, &QAction::triggered, this, &UiManager::switchToEditorMode);

	mMainWindow.statusBar()->setStyleSheet("QStatusBar::item { border: 0px solid black; padding: 10px; }");
	editModeAction.setProperty("modeName", tr("edit mode"));
	debugModeAction.setProperty("modeName", tr("debug mode"));
	produceModeButton(Mode::Editing, debugModeAction, mMainWindow.statusBar());
	produceModeButton(Mode::Debugging, editModeAction, mMainWindow.statusBar());

	switchToEditorMode();
}

void UiManager::placeDevicesConfig(QWidget *devicesWidget)
{
	QDockWidget * const devicesDock = produceDockWidget(QObject::tr("Configure devices"), devicesWidget);
	devicesDock->setObjectName("devicesConfigurationDock");
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

	mMainWindow.tabifyDockWidget(watchWindow, graphWatchDock);
	reloadDocks();
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
	reloadDocks();
	toggleModeButtons();
}

void UiManager::toggleModeButtons()
{
	mEditModeAction.setVisible(mCurrentMode == Mode::Debugging && mCurrentTab != qReal::TabInfo::TabType::other);
	mDebugModeAction.setVisible(mCurrentMode == Mode::Editing && mCurrentTab != qReal::TabInfo::TabType::other);

	const QColor color = mCurrentTab == qReal::TabInfo::TabType::other
			? backgrondColor
			: mCurrentMode == Mode::Editing ? editModeColor : debugModeColor;
	QPalette palette;
	palette.setColor(QPalette::Background, color);
	mMainWindow.statusBar()->setPalette(palette);
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

	statusBar->addWidget(result, 10);
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
	const QByteArray state = qReal::SettingsManager::value(currentSettingsKey()).toByteArray();
	if (!mMainWindow.restoreState(state, currentMode())) {
		QLOG_ERROR() << "Cannot apply docks state for mode" << currentMode() << ":" << state;
	}
}

void UiManager::addWidgetsForDocksDebugging() const
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
