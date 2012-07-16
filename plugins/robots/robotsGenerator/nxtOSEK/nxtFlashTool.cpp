#include "nxtFlashTool.h"

#include <QtCore/QCoreApplication>

using namespace qReal;
using namespace robots::generator;

NxtFlashTool::NxtFlashTool(qReal::ErrorReporterInterface *errorReporter)
		: mErrorReporter(errorReporter)
		, mUploadState(done)
{
	QProcessEnvironment environment(QProcessEnvironment::systemEnvironment());
	environment.insert("QREALDIR", qApp->applicationDirPath());
	environment.insert("DISPLAY", ":0.0");
	mFlashProcess.setProcessEnvironment(environment);
	mUploadProcess.setProcessEnvironment(environment);

	// for debug
	mUploadProcess.setStandardErrorFile("./errors");
	mUploadProcess.setStandardOutputFile("./out");

	connect(&mFlashProcess, SIGNAL(readyRead()), this, SLOT(readNxtFlashData()));
	connect(&mFlashProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
	connect(&mFlashProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(nxtFlashingFinished(int, QProcess::ExitStatus)));

	connect(&mUploadProcess, SIGNAL(readyRead()), this, SLOT(readNxtUploadData()));
	connect(&mUploadProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
	connect(&mUploadProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(nxtUploadingFinished(int, QProcess::ExitStatus)));
}

void NxtFlashTool::flashRobot()
{
#ifdef Q_OS_WIN
	mFlashProcess.setEnvironment(QProcess::systemEnvironment());
	mFlashProcess.setWorkingDirectory(qApp->applicationDirPath() + "/nxt-tools/nexttool/");
	mFlashProcess.start("cmd", QStringList() << "/c" << qApp->applicationDirPath() + "/nxt-tools/flash.bat");
#else
	mFlashProcess.start("sh", QStringList() << qApp->applicationDirPath() + "/nxt-tools/flash.sh");
#endif

	mErrorReporter->addInformation(tr("Firmware flash started. Please don't disconnect robot during the process"));
}

void NxtFlashTool::error(QProcess::ProcessError error)
{
	qDebug() << "error:" << error;
	mErrorReporter->addInformation(tr("Some error occured. Make sure you are running QReal with superuser privileges"));
}

void NxtFlashTool::nxtFlashingFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	qDebug() << "finished with code " << exitCode << ", status: " << exitStatus;

	if (exitCode == 0) {
		mErrorReporter->addInformation(tr("Flashing process completed."));
	} else if (exitCode == 127) {
		mErrorReporter->addError(tr("flash.sh not found. Make sure it is present in QReal installation directory"));
	} else if (exitCode == 139) {
		mErrorReporter->addError(tr("QReal requires superuser privileges to flash NXT robot"));
	}
}

void NxtFlashTool::readNxtFlashData()
{
	QStringList const output = QString(mFlashProcess.readAll()).split("\n", QString::SkipEmptyParts);

	qDebug() << "exit code:" << mFlashProcess.exitCode();
	qDebug() << output;

	foreach (QString error, output) {
		if (error == "NXT not found. Is it properly plugged in via USB?") {
			mErrorReporter->addError(tr("NXT not found. Check USB connection and make sure the robot is ON"));
		} else if (error == "NXT found, but not running in reset mode.") {
			mErrorReporter->addError(tr("NXT is not in reset mode. Please reset your NXT manually and try again"));
		} else if (error == "Firmware flash complete.") {
			mErrorReporter->addInformation(tr("Firmware flash complete!"));
		}
	}
}

void NxtFlashTool::uploadProgram()
{
#ifdef Q_OS_WIN
	mUploadProcess.setEnvironment(QProcess::systemEnvironment());
	mUploadProcess.setWorkingDirectory(qApp->applicationDirPath() + "/nxt-tools/");
	mUploadProcess.start("cmd", QStringList() << "/c" << qApp->applicationDirPath() + "/nxt-tools/upload.bat");
#else
	mUploadProcess.start("sh", QStringList() << qApp->applicationDirPath() + "/nxt-tools/upload.sh");
#endif

	mErrorReporter->addInformation(tr("Uploading program started. Please don't disconnect robot during the process"));
}

void NxtFlashTool::nxtUploadingFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	qDebug() << "finished uploading with code " << exitCode << ", status: " << exitStatus;

	if (exitCode == 127) { // most likely wineconsole didn't start and generate files needed to proceed compilation
		mErrorReporter->addError(tr("Uploading failed. Make sure that X-server allows root to run GUI applications"));
	} else if (exitCode == 139) {
		mErrorReporter->addError(tr("QReal requires supruser privileges to flash NXT robot"));
	}
}

void NxtFlashTool::readNxtUploadData()
{
	QStringList const output = QString(mUploadProcess.readAll()).split("\n", QString::SkipEmptyParts);

	qDebug() << "exit code:" << mUploadProcess.exitCode();
	qDebug() << output;

	/* each command produces its own output, so thousands of 'em. using UploadState enum
	   to determine in which state we are (to show appropriate error if something goes wrong)
	*/

	foreach (QString error, output) {
		if (error.contains("Removing ")) {
			mUploadState = clean;
		} else if (error.contains("Compiling ")) {
			mUploadState = compile;
		} else if (error.contains("Generating binary image file")) {
			mUploadState = link;
		} else if (error.contains("Executing NeXTTool to upload")) {
			mUploadState = uploadStart;
		} else if (error.contains("_OSEK.rxe=")) {
			mUploadState = flash;
		} else if (error.contains("NeXTTool is terminated")) {
			if (mUploadState == uploadStart) {
				mErrorReporter->addError(tr("Could not upload program. Make sure the robot is connected and ON"));
			} else if (mUploadState == flash) {
				mErrorReporter->addInformation(tr("Uploading completed successfully"));
			}
			mUploadState = done;
		}
		else if (error.contains("An unhandled exception occurred")) {
			mErrorReporter->addError(tr("QReal requires superuser privileges to upload programs on NXT robot"));
			break;
		}
	}
	qDebug() << "NxtFlashTool::readNxtUploadData" << mUploadState;
}

