#pragma once

#include <QtCore/QProcess>
#include <ev3GeneratorBase/ev3GeneratorPluginBase.h>

namespace ev3 {
namespace rbf {

class Ev3RbfGeneratorPlugin : public Ev3GeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "ev3.Ev3RbfGeneratorPlugin")

public:
	Ev3RbfGeneratorPlugin();

	QList<qReal::ActionInfo> actions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	void regenerateExtraFiles(QFileInfo const &newFileInfo) override;
	QString defaultFilePath(QString const &projectName) const override;
	QString extension() const override;
	QString extensionDescription() const override;
	QString generatorName() const override;

private slots:

	/// Generates and uploads script to a robot.
	/// @returns True, if successful.
	bool uploadProgram();

private:
	///Function that checks installed JRE or not
	bool javaInstalled();

	/// Action that launches code generator
	QAction *mGenerateCodeAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that generates and uploads program on a robot
	QAction *mUploadProgramAction;  // Doesn't have ownership; may be disposed by GUI.
};

}
}
