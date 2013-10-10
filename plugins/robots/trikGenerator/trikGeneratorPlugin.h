#pragma once

#include <QtCore/QTranslator>

#include "qrgui/toolPluginInterface/toolPluginInterface.h"
#include "qrgui/toolPluginInterface/pluginConfigurator.h"
#include "qrrepo/repoApi.h"

namespace qReal {
namespace robots {
namespace generators {
namespace trik {

/// Generation of QtScript program for TRIK, uploading and execution of a program.
/// Uses settings "tcpServer" and "tcpPort" from RobotsInterpreter.
class TrikGeneratorPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.robots.trikGenerator.TrikGeneratorPlugin")

public:
	TrikGeneratorPlugin();

	virtual ~TrikGeneratorPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);

	virtual QList<qReal::ActionInfo> actions();

private slots:
	/// Calls code generator. Returns true if operation was successful.
	bool generateCode();

	/// Generates and uploads script to a robot. Program then can be launched manually or remotely
	/// by runCommand. Program is stored on robot as a file next to scriptRunner and named
	/// as <qReal save name>.qts.
	/// @returns True, if successful.
	bool uploadProgram();

	/// Runs currently opened program on a robot. Uploads it first.
	void runProgram();

	/// Tries to remotely abort script execution and stop robot.
	void stopRobot();

private:
	/// Returns filename of a currently generated program.
	QString currentProgramName() const;

	/// Action that launches code generator
	QAction mGenerateCodeAction;

	/// Action that generates and uploads program on a robot
	QAction mUploadProgramAction;

	/// Action that generates and uploads program on a robot
	QAction mRunProgramAction;

	/// Action that stops script execution and turns off motors.
	QAction mStopRobotAction;

	/// Translator object for this plugin
	QTranslator mAppTranslator;

	/// Interface of MainWindow
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// Interface of project manager (allows to perform open/save activities)
	qReal::ProjectManagementInterface *mProjectManager; // Does not have ownership

	/// Control interface of the repository
	qrRepo::RepoApi const *mRepo;  // Does not have ownership
};

}
}
}
}
