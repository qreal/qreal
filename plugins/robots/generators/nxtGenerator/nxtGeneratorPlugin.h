#pragma once

#include <QtCore/QTranslator>
#include <QtCore/QMultiHash>

#include <qrgui/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/toolPluginInterface/pluginConfigurator.h>
#include <qrgui/toolPluginInterface/hotKeyActionInfo.h>
#include <qrgui/toolPluginInterface/systemEventsInterface.h>
#include <qrgui/textEditor/textManagerInterface.h>
#include <generatorBase/robotsGeneratorPluginBase.h>
#include "nxtFlashTool.h"

namespace nxtOsek {

/// Main plugin class for NXT code generator. Provides generator that generates
/// C code for nxtOSEK from robot diagrams, and interface for standalone nxt-tools
/// toolset, used to compile generated sources and flash them to robot, or
/// install nxtOSEK on a robot.
class NxtGeneratorPlugin : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "nxtOsek.NxtGeneratorPlugin")

public:
	NxtGeneratorPlugin();
	virtual ~NxtGeneratorPlugin();

	QString kitId() const override;

	void init(qReal::PluginConfigurator const &configurator
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager) override;
	QList<qReal::ActionInfo> actions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	void regenerateExtraFiles(QFileInfo const &newFileInfo) override;
	QString defaultFilePath(QString const &projectName) const override;
	QString extension() const override;
	QString extDescrition() const override;
	QString generatorName() const override;
	bool canGenerateTo(QString const &project) override;

private slots:
	/// Uploads and installs nxtOSEK on a robot. Requires nxt-tools.
	void flashRobot();

	/// Compiles and uploads program to a robot. Program then can be launched manually.
	/// Requires nxt-tools
	void uploadProgram();

	/// Runs program if it is allowed in settings
	void onUploadingComplete(bool success);

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
