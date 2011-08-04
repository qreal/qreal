#include "nxtFlashTool.h"

using namespace qReal;
using namespace gui;

NxtFlashTool::NxtFlashTool(ErrorReporter *errorReporter)
	: mErrorReporter(errorReporter)
{
	QProcessEnvironment environment;
	environment.insert("QREALDIR", qApp->applicationDirPath());
	mFlashProcess.setProcessEnvironment(environment);

	connect(&mFlashProcess, SIGNAL(readyRead()), this, SLOT(readNxtFlashData()));
	connect(&mFlashProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
	connect(&mFlashProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(nxtFlashingFinished(int, QProcess::ExitStatus)));
}

void NxtFlashTool::flashRobot()
{
#ifdef Q_OS_UNIX
	mFlashProcess.start("sh", QStringList() << qApp->applicationDirPath() + "/generators/nxt-tools/linux/flash.sh");
#endif

#ifdef Q_OS_WIN
	QString path = qApp->applicationDirPath();
	path.replace(QRegExp("/"), "\\");
	qDebug() << path;
	task.start("cmd", QStringList() << path + "\\generators\\windows\\flash.bat");
#endif

	mErrorReporter->addInformation("Firmware flash started. Please don't disconnect robot during the process");

	emit showErrors(mErrorReporter);
}

void NxtFlashTool::error(QProcess::ProcessError error)
{
	qDebug() << "error:" << error;
}

void NxtFlashTool::nxtFlashingFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	qDebug() << "finished with code " << exitCode << ", status: " << exitStatus;

	if (exitCode == 127)
		mErrorReporter->addError("flash.sh not found. Make sure it is present in QReal installation directory");
	else if (exitCode == 139)
		mErrorReporter->addError("QReal requires superuser privileges to flash NXT robot");

	emit showErrors(mErrorReporter);
}

void NxtFlashTool::readNxtFlashData()
{
	QStringList output = QString(mFlashProcess.readAll()).split("\n", QString::SkipEmptyParts);

	qDebug() << "exit code:" << mFlashProcess.exitCode();
	qDebug() << output;

	foreach (QString error, output){
		if (error == "NXT not found. Is it properly plugged in via USB?")
			mErrorReporter->addError("NXT not found. Check USB connection and make sure the robot is ON");
		else if (error == "NXT found, but not running in reset mode.")
			mErrorReporter->addError("NXT is not in reset mode. Please reset your NXT manually and try again");
		else if (error == "Firmware flash complete.")
			mErrorReporter->addInformation("Firmware flash complete!");
	}

	emit showErrors(mErrorReporter);
}

void NxtFlashTool::uploadProgram()
{

	QProcess *task = new QProcess(this);
#ifdef Q_OS_UNIX
	task->start("sh", QStringList() << QDir::currentPath() + "/generators/linux/upload.sh");
#endif

#ifdef Q_OS_WIN
	QString path = QDir::currentPath();
	path.replace(QRegExp("/"), "\\");
	task->start("cmd", QStringList() << path + "\\generators\\windows\\upload.bat");
#endif

	delete task;
}
