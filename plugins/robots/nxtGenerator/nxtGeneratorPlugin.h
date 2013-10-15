#pragma once

#include <robotsGeneratorPluginBase.h>
#include "nxtFlashTool.h"

namespace qReal {
namespace robots {
namespace generators {

/// Main plugin class for NXT code generator. Provides generator that generates
/// C code for nxtOSEK from robot diagrams, and interface for standalone nxt-tools
/// toolset, used to compile generated sources and flash them to robot, or
/// install nxtOSEK on a robot.
class NxtGeneratorPlugin : public RobotsGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "qReal.robots.generator.NxtGeneratorPlugin")

public:
	NxtGeneratorPlugin();
	virtual ~NxtGeneratorPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();
	virtual QList<qReal::HotKeyActionInfo> hotKeyActions();

protected:
	virtual MasterGeneratorBase *masterGenerator();

private slots:
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

	/// When true, nxt-tools are found by QReal and flashing and uploading is possible
	bool mNxtToolsPresent;

	/// Flasher object
	NxtFlashTool *mFlashTool;  // Has ownership

	QList<qReal::HotKeyActionInfo> mHotKeyActionInfos;

	QTranslator mAppTranslator;
};

}
}
}
