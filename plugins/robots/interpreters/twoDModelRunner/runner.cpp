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

#include <twoDModel/engine/view/d2ModelWidget.h>

using namespace twoDModel;

Runner::Runner()
	: mProjectManager(mQRealFacade.models())
	, mMainWindow(mErrorReporter, mQRealFacade.events()
			, mProjectManager, mQRealFacade.models().graphicalModelAssistApi())
	, mConfigurator(mQRealFacade.models().repoControlApi()
			, mQRealFacade.models().graphicalModelAssistApi()
			, mQRealFacade.models().logicalModelAssistApi()
			, mMainWindow
			, mProjectManager
			, mSceneCustomizer
			, mQRealFacade.events()
			, mTextManager)
{
	mPluginFacade.customizer().customizeDocks(&mMainWindow);
	mPluginFacade.init(mConfigurator);
	for (const QString &defaultSettingsFile : mPluginFacade.defaultSettingsFiles()) {
		qReal::SettingsManager::loadDefaultSettings(defaultSettingsFile);
	}
}

void Runner::interpret(const QString &saveFile, bool background)
{
	if (!mProjectManager.open(saveFile)) {
		return;
	}

	/// @todo: A bit hacky way to get 2D model window. Actually we must not have need in this.
	/// GUI must be separated from logic and not appear here at all.
	QList<view::D2ModelWidget *> twoDModelWindows;
	for (QWidget * const widget : QApplication::topLevelWidgets()) {
		if (view::D2ModelWidget * const  twoDModelWindow = dynamic_cast<view::D2ModelWidget *>(widget)) {
			twoDModelWindows << twoDModelWindow;
		}
	}

	for (view::D2ModelWidget * const  twoDModelWindow : twoDModelWindows) {
		QObject::connect(twoDModelWindow, &view::D2ModelWidget::widgetClosed
				, &mMainWindow, &qReal::NullMainWindow::emulateClose);

		if (background) {
			twoDModelWindow->setBackgroundMode(true);
			QObject::connect(&mPluginFacade.interpreter(), &interpreterCore::interpreter::InterpreterInterface::stopped
					, &mMainWindow, &qReal::NullMainWindow::emulateClose);
		}
	}

	mPluginFacade.actionsManager().runAction().trigger();
}
