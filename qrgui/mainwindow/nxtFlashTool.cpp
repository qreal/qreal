#include "nxtFlashTool.h"

using namespace qReal;
using namespace gui;

NxtFlashTool::NxtFlashTool(ErrorReporter *errorReporter)
	: mErrorReporter(errorReporter)
{
	QProcessEnvironment environment;
	environment.insert("QREALDIR", qApp->applicationDirPath());
	environment.insert("DISPLAY", ":0.0");
	mFlashProcess.setProcessEnvironment(environment);
	mUploadProcess.setProcessEnvironment(environment);

	// for debug
//	mUploadProcess.setStandardErrorFile("/home/me/downloads/incoming/errors");
//	mUploadProcess.setStandardOutputFile("/home/me/downloads/incoming/out");

	connect(&mFlashProcess, SIGNAL(readyRead()), this, SLOT(readNxtFlashData()));
	connect(&mFlashProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
	connect(&mFlashProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(nxtFlashingFinished(int, QProcess::ExitStatus)));

	connect(&mUploadProcess, SIGNAL(readyRead()), this, SLOT(readNxtUploadData()));
	connect(&mUploadProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
	connect(&mUploadProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(nxtUploadingFinished(int, QProcess::ExitStatus)));

}

void NxtFlashTool::flashRobot()
{
#ifdef Q_OS_UNIX
	mFlashProcess.start("sh", QStringList() << qApp->applicationDirPath() + "/nxt-tools/flash.sh");
#endif

#ifdef Q_OS_WIN
	QString path = qApp->applicationDirPath();
	path.replace(QRegExp("/"), "\\");
	qDebug() << path;
	mFlashProcess.start("cmd", QStringList() << path + "\\nxt-tools\\flash.bat");
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

#ifdef Q_OS_UNIX
	mUploadProcess.start("sh", QStringList() << QDir::currentPath() + "/nxt-tools/upload.sh");
#endif

#ifdef Q_OS_WIN
	QString path = QDir::currentPath();
	path.replace(QRegExp("/"), "\\");
	mUploadProcess->start("cmd", QStringList() << path + "\\nxt-tools\\upload.bat");
#endif

	mErrorReporter->addInformation("Firmware flash started. Please don't disconnect robot during the process");
	emit showErrors(mErrorReporter);
}


void NxtFlashTool::nxtUploadingFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	qDebug() << "finished uploading with code " << exitCode << ", status: " << exitStatus;

	if (exitCode == 127)
		mErrorReporter->addError("Something went wrong :)");
	else if (exitCode == 139)
		mErrorReporter->addError("QReal requires superuser privileges to flash NXT robot");

	emit showErrors(mErrorReporter);
}

void NxtFlashTool::readNxtUploadData()
{
	QStringList output = QString(mUploadProcess.readAll()).split("\n", QString::SkipEmptyParts);

	qDebug() << "exit code:" << mUploadProcess.exitCode();
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
