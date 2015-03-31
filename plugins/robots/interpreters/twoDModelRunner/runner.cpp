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
			twoDModelWindow->setImmediateMode(true);
			QObject::connect(&mPluginFacade.interpreter(), &interpreterCore::interpreter::InterpreterInterface::stopped
					, &mMainWindow, &qReal::NullMainWindow::emulateClose);
		}
	}

	mPluginFacade.actionsManager().runAction().trigger();
}
