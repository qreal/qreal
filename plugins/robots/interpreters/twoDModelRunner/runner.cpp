/* Copyright 2007-2015 QReal Research Group
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

#include "runner.h"

#include <QtWidgets/QApplication>

#include <twoDModel/engine/view/twoDModelWidget.h>
#include <twoDModel/engine/model/model.h>

using namespace twoDModel;

Runner::Runner(const QString &report, const QString &trajectory)
	: mProjectManager(mQRealFacade.models())
	, mMainWindow(mErrorReporter, mQRealFacade.events()
			, mProjectManager, mQRealFacade.models().graphicalModelAssistApi())
	, mConfigurator(mQRealFacade.models().repoControlApi()
			, mQRealFacade.models().graphicalModelAssistApi()
			, mQRealFacade.models().logicalModelAssistApi()
			, mMainWindow
			, mMainWindow
			, mProjectManager
			, mSceneCustomizer
			, mQRealFacade.events()
			, mTextManager)
	, mReporter(report, trajectory)
{
	mPluginFacade.init(mConfigurator);
	for (const QString &defaultSettingsFile : mPluginFacade.defaultSettingsFiles()) {
		qReal::SettingsManager::loadDefaultSettings(defaultSettingsFile);
	}

	connect(&mErrorReporter, &qReal::ConsoleErrorReporter::informationAdded, &mReporter, &Reporter::addInformation);
	connect(&mErrorReporter, &qReal::ConsoleErrorReporter::errorAdded, &mReporter, &Reporter::addError);
	connect(&mErrorReporter, &qReal::ConsoleErrorReporter::criticalAdded, &mReporter, &Reporter::addError);
}

Runner::~Runner()
{
	mReporter.reportMessages();
}

void Runner::interpret(const QString &saveFile, bool background)
{
	if (!mProjectManager.open(saveFile)) {
		return;
	}

	/// @todo: A bit hacky way to get 2D model window. Actually we must not have need in this.
	/// GUI must be separated from logic and not appear here at all.
	QList<view::TwoDModelWidget *> twoDModelWindows;
	for (QWidget * const widget : QApplication::allWidgets()) {
		if (view::TwoDModelWidget * const twoDModelWindow = dynamic_cast<view::TwoDModelWidget *>(widget)) {
			twoDModelWindows << twoDModelWindow;
		}
	}

	if (background) {
		connect(&mPluginFacade.interpreter(), &interpreterCore::interpreter::InterpreterInterface::stopped
				, [=]() { mMainWindow.emulateClose(mReporter.lastMessageIsError() ? 1 : 0); });
	}

	for (view::TwoDModelWidget * const  twoDModelWindow : twoDModelWindows) {
		connect(twoDModelWindow, &view::TwoDModelWidget::widgetClosed, [=]() { mMainWindow.emulateClose(); });
		twoDModelWindow->model().timeline().setImmediateMode(background);
		for (model::RobotModel *robotModel : twoDModelWindow->model().robotModels()) {
			connect(robotModel, &model::RobotModel::robotRided, this, &Runner::onRobotRided, Qt::UniqueConnection);
		}
	}

	mPluginFacade.actionsManager().runAction().trigger();
}

void Runner::onRobotRided(const QPointF &newPosition, const qreal newRotation)
{
	mReporter.newTrajectoryPoint(
			static_cast<model::RobotModel *>(sender())->info().robotId()
			, mPluginFacade.interpreter().timeElapsed()
			, newPosition
			, newRotation
	);
}
