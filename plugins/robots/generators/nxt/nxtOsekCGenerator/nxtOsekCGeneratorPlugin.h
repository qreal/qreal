#pragma once

#include <nxtGeneratorBase/nxtGeneratorPluginBase.h>

#include "nxtFlashTool.h"

namespace nxt {
namespace osekC {

class NxtOsekCMasterGenerator;

/// Main plugin class for NXT code generator. Provides generator that generates
/// C code for nxtOSEK from robot diagrams, and interface for standalone nxt-tools
/// toolset, used to compile generated sources and flash them to robot, or
/// install nxtOSEK on a robot.
class NxtOsekCGeneratorPlugin : public NxtGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "nxt.NxtOsekCGeneratorPlugin")

public:
	NxtOsekCGeneratorPlugin();
	~NxtOsekCGeneratorPlugin() override;

	void init(const qReal::PluginConfigurator &configurator
			, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage) override;
	QList<qReal::ActionInfo> actions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	void regenerateExtraFiles(QFileInfo const &newFileInfo) override;
	QString defaultFilePath(const QString &projectName) const override;
	qReal::text::LanguageInfo language() const override;
	QString generatorName() const override;
	bool canGenerateTo(const QString &project) override;

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

	void initHotKeyActions();

	/// Action that launches code generator
	QAction *mGenerateCodeAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that uploads nxtOSEK on a robot
	QAction *mFlashRobotAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that compiles and uploads program on a robot
	QAction *mUploadProgramAction;  // Doesn't have ownership; may be disposed by GUI.

	QList<qReal::HotKeyActionInfo> mHotKeyActionInfos;

	/// When true, nxt-tools are found by QReal and flashing and uploading is possible
	bool mNxtToolsPresent;
	/// Flasher object
	NxtFlashTool *mFlashTool = nullptr;  // Has ownership

	NxtOsekCMasterGenerator *mMasterGenerator;
};

}
}
