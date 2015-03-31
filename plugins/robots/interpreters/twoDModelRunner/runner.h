#pragma once

#include <qrgui/systemFacade/systemFacade.h>
#include <qrgui/systemFacade/components/consoleErrorReporter.h>
#include <qrgui/systemFacade/components/nullMainWindow.h>
#include <qrgui/systemFacade/components/projectManager.h>
#include <qrgui/systemFacade/components/nullTextManager.h>
#include <qrgui/editor/sceneCustomizer.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <interpreterCore/robotsPluginFacade.h>

namespace twoDModel {

/// Creates instances null QReal environment, of robots plugin and runs interpretation on 2D model window.
class Runner
{
public:
	Runner();

	/// Starts the interpretation process. The given save file will be opened and interpreted in 2D model window.
	/// @param background If true then the save file will be interpreted in the fastest speed and 2D model window
	/// will be closed immediately after the interpretation stopped.
	void interpret(const QString &saveFile, bool background);

private:
	qReal::SystemFacade mQRealFacade;
	qReal::ConsoleErrorReporter mErrorReporter;
	qReal::ProjectManager mProjectManager;
	qReal::NullMainWindow mMainWindow;
	qReal::NullTextManager mTextManager;
	qReal::SceneCustomizer mSceneCustomizer;
	qReal::PluginConfigurator mConfigurator;
	interpreterCore::RobotsPluginFacade mPluginFacade;
};

}
