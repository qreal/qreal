#pragma once

#include <trikGeneratorBase/trikGeneratorPluginBase.h>
#include <trikGeneratorBase/robotModel/generatorModelExtensionInterface.h>

namespace utils {
class TcpRobotCommunicator;
}

namespace trik {
namespace pascalABC {

///Generation of PascalABC program for TRIK, uploading and execution a program
///Uses setting "tcpServer" from RobotsInterpreter.
class TrikPascalABCGeneratorPluginBase : public TrikGeneratorPluginBase
{
	Q_OBJECT

public:
	TrikPascalABCGeneratorPluginBase(kitBase::robotModel::RobotModelInterface * const robotModel
									 , kitBase::blocksBase::BlocksFactoryInterface * const blocksFactory
									 , const QStringList &pathsToTemplates);

	~TrikPascalABCGeneratorPluginBase() override;

	//?init

	QList<qReal::ActionInfo> customActions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;
	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	QString defaultFilePath(const QString &projectName) const override;
	qReal::text::LanguageInfo language() const override;
	QString generatorName() const override;

	//?addShellDevice

private slots:
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
	/// Action that launches code generator
	QAction *mGenerateCodeAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that generates and uploads program on a robot
	QAction *mUploadProgramAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that generates and uploads program on a robot
	QAction *mRunProgramAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that stops script execution and turns off motors.
	QAction *mStopRobotAction;  // Doesn't have ownership; may be disposed by GUI.

	//?TCP communicator

	QStringList mPathsToTemplates;
};
}
}
