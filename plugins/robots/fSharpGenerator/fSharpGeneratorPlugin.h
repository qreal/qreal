#pragma once

#include <robotsGeneratorPluginBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace fSharp {

/// Generation of QtScript program for FSharp, uploading and execution of a program.
/// Uses setting "tcpServer" from RobotsInterpreter.
class FSharpGeneratorPlugin : public RobotsGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "qReal.robots.FSharpGenerator.FSharpGeneratorPlugin")

public:
	FSharpGeneratorPlugin();

	virtual ~FSharpGeneratorPlugin();

	virtual QList<qReal::ActionInfo> actions();

protected:
	virtual MasterGeneratorBase *masterGenerator();
	virtual void regenerateExtraFiles(QFileInfo const &newFileInfo);
	virtual QString defaultFilePath(QString const &projectName) const;
	virtual QString extension() const;
	virtual QString extDescrition() const;
	virtual QString generatorName() const;

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
	QAction mGenerateCodeAction;

	/// Action that generates and uploads program on a robot
	QAction mUploadProgramAction;

	/// Action that generates and uploads program on a robot
	QAction mRunProgramAction;

	/// Action that stops script execution and turns off motors.
	QAction mStopRobotAction;

	QTranslator mAppTranslator;
};

}
}
}
}
