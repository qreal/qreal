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

#include <qrkernel/logging.h>
#include <qrkernel/settingsManager.h>

#include <QtWidgets/QAction>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

using namespace interpreterCore;

UiManager::UiManager(QAction &debugModeAction
		, QAction &editModeAction
		, qReal::gui::MainWindowDockInterface &mainWindow
		, qReal::SystemEvents &systemEvents
		, kitBase::EventsForKitPluginInterface &kitPluginEvents)
	: mDebugModeAction(debugModeAction)
	, mEditModeAction(editModeAction)
	, mMainWindow(mainWindow)
	, mSystemEvents(systemEvents)
	, mKitPluginEvents(kitPluginEvents)
{
	mMainWindow.graphicalModelDock()->setWindowTitle(QObject::tr("Blocks"));

	connect(&mKitPluginEvents, &kitBase::EventsForKitPluginInterface::interpretationStarted
			, this, &UiManager::switchToDebuggerMode);
	connect(&mDebugModeAction, &QAction::triggered, this, &UiManager::switchToDebuggerMode);
	connect(&mEditModeAction, &QAction::triggered, this, &UiManager::switchToEditorMode);
	switchToEditorMode();

	addWidgetsForDocksDebugging();
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
}

void UiManager::onActiveTabChanged(const qReal::TabInfo &tab)
{
	saveDocks();
	mCurrentTab = tab.type();
	reloadDocks();
}

void UiManager::switchToEditorMode()
{
	saveDocks();
	mCurrentMode = Mode::Editing;
	reloadDocks();
	mEditModeAction.setVisible(false);
	mDebugModeAction.setVisible(true);
}

void UiManager::switchToDebuggerMode()
{
	saveDocks();
	mCurrentMode = Mode::Debugging;
	reloadDocks();
	mEditModeAction.setVisible(true);
	mDebugModeAction.setVisible(false);
}

QDockWidget *UiManager::produceDockWidget(const QString &title, QWidget *content) const
{
	QDockWidget * const dock = new QDockWidget(title);
	dock->setWidget(content);
	return dock;
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
	QPushButton * const button = new QPushButton("Snapshot docks", mainWindow);
	button->move(mainWindow->geometry().center());
	QLineEdit * const lineEdit = new QLineEdit(mainWindow);
	lineEdit->move(button->geometry().bottomLeft());
	connect(button, &QPushButton::clicked, [=]() {
		lineEdit->setText(mMainWindow.saveState(currentMode()));
	});
	button->show();
	lineEdit->show();
}
