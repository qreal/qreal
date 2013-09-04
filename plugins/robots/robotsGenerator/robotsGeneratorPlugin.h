#pragma once

#include <QtCore/QTranslator>

#include <qrgui/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/toolPluginInterface/pluginConfigurator.h>
#include <qrgui/toolPluginInterface/hotKeyActionInfo.h>
#include <qrrepo/repoApi.h>
#include "nxtOSEK/nxtFlashTool.h"


namespace qReal {
namespace robots {
namespace generators {

/// Main plugin class for NXT code generator. Provides generator that generates
/// C code for nxtOSEK from robot diagrams, and interface for standalone nxt-tools
/// toolset, used to compile generated sources and flash them to robot, or
/// install nxtOSEK on a robot.
class RobotsGeneratorPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.robots.generator.RobotsGeneratorPlugin")

public:
	RobotsGeneratorPlugin();
	virtual ~RobotsGeneratorPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();
	virtual QList<qReal::HotKeyActionInfo> hotKeyActions();

private slots:
	/// Calls code generator. Returns true if operation was successfull.
	bool generateRobotSourceCode();

	/// Uploads and installs nxtOSEK on a robot. Requires nxt-tools.
	void flashRobot();

	/// Compiles and uploads program to a robot. Program then can be launched manually.
	/// Requires nxt-tools
	void uploadProgram();

private:
	/// Method that checks presense of nxt-tools (shall be installed to nxt-tools
	/// subfolder of QReal installation), and sets mNxtToolsPresent flag.
	void checkNxtTools();

	/// Disable/enable tab in QList<ActionInfo> info
	void changeActiveTab(QList<qReal::ActionInfo> const &info, bool const &trigger);

	void initHotKeyActions();

	/// Action that launches code generator
	QAction mGenerateCodeAction;

	/// Action that uploads nxtOSEK on a robot
	QAction mFlashRobotAction;

	/// Action that compiles and uploads program on a robot
	QAction mUploadProgramAction;

	/// Translator object for this plugin
	QTranslator mAppTranslator;

	/// Interface of MainWindow
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// Interface of project manager (allows to perform open/save activities)
	qReal::ProjectManagementInterface *mProjectManager; // Does not have ownership

	/// Repository API
	qrRepo::RepoApi const *mRepo;  // Does not have ownership

	/// When true, nxt-tools are found by QReal and flashing and uploading is possible
	bool mNxtToolsPresent;

	/// Flasher object
	NxtFlashTool *mFlashTool;  // Has ownership

	QList<qReal::HotKeyActionInfo> mHotKeyActionInfos;
};

}
}
}
