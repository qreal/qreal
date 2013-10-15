#pragma once

#include <robotsGeneratorPluginBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace trik {

/// Generation of QtScript program for TRIK, uploading and execution of a program.
/// Uses settings "tcpServer" and "tcpPort" from RobotsInterpreter.
class TrikGeneratorPlugin : public RobotsGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "qReal.robots.trikGenerator.TrikGeneratorPlugin")

public:
	TrikGeneratorPlugin();

	virtual ~TrikGeneratorPlugin();

	virtual QList<qReal::ActionInfo> actions();

protected:
	virtual MasterGeneratorBase *masterGenerator();

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

	QTranslator mAppTranslator;
};

}
}
}
}
