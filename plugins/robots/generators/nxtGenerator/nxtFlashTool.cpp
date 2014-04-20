#include "nxtFlashTool.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtWidgets/QMessageBox>

using namespace nxtOsek;
using namespace qReal;

NxtFlashTool::NxtFlashTool(qReal::ErrorReporterInterface *errorReporter)
		: mErrorReporter(errorReporter)
		, mIsFlashing(false)
		, mIsUploading(false)
		, mUploadState(done)
{
	QProcessEnvironment environment(QProcessEnvironment::systemEnvironment());
	environment.insert("QREALDIR", qApp->applicationDirPath());
	environment.insert("QREALDIRPOSIX", qApp->applicationDirPath().remove(1, 1).prepend("/cygdrive/"));
	environment.insert("DISPLAY", ":0.0");
	mFlashProcess.setProcessEnvironment(environment);
	mUploadProcess.setProcessEnvironment(environment);
	mRunProcess.setProcessEnvironment(environment);

	connect(&mFlashProcess, SIGNAL(readyRead()), this, SLOT(readNxtFlashData()));
	connect(&mFlashProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
	connect(&mFlashProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(nxtFlashingFinished(int, QProcess::ExitStatus)));

	connect(&mUploadProcess, SIGNAL(readyRead()), this, SLOT(readNxtUploadData()));
	connect(&mUploadProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
	connect(&mUploadProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(nxtUploadingFinished(int, QProcess::ExitStatus)));

	connect(&mRunProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
}

void NxtFlashTool::flashRobot()
{
	if (mIsFlashing) {
		mErrorReporter->addInformation(tr("Robot is already being flashed"));
		return;
	}

	mIsFlashing = true;

#ifdef Q_OS_WIN
	mFlashProcess.setEnvironment(QProcess::systemEnvironment());
	mFlashProcess.setWorkingDirectory(qApp->applicationDirPath() + "/nxt-tools/nexttool/");
	mFlashProcess.start("cmd", QStringList() << "/c" << qApp->applicationDirPath() + "/nxt-tools/flash.bat");
#else
	mFlashProcess.start("sh", QStringList() << qApp->applicationDirPath() + "/nxt-tools/flash.sh");
#endif

	mErrorReporter->addInformation(tr("Firmware flash started. Please don't disconnect robot during the process"));
}

void NxtFlashTool::runProgram(QFileInfo const &fileInfo)
{
	mSource = fileInfo;
	mRunProcess.setEnvironment(QProcess::systemEnvironment());
	mRunProcess.setWorkingDirectory(qApp->applicationDirPath() + "/nxt-tools/");
	mRunProcess.start("cmd", QStringList() << "/c" << qApp->applicationDirPath() + "/nxt-tools/nexttool/NexTTool.exe /COM=usb -run="
			+ QString("%1_OSEK.rxe").arg(mSource.baseName()));
}

void NxtFlashTool::runLastProgram()
{
	if (mSource != QFileInfo()) {
		runProgram(mSource);
	}
}

void NxtFlashTool::error(QProcess::ProcessError error)
{
	Q_UNUSED(error)
	mErrorReporter->addInformation(tr("Some error occured. Make sure you are running QReal with superuser privileges"));
}

void NxtFlashTool::nxtFlashingFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus)

	mIsFlashing = false;

	if (exitCode == 0) {
		mErrorReporter->addInformation(tr("Flashing process completed."));
	} else if (exitCode == 127) {
		mErrorReporter->addError(tr("flash.sh not found. Make sure it is present in QReal installation directory"));
	} else if (exitCode == 139) {
		mErrorReporter->addError(tr("QReal requires superuser privileges to flash NXT robot"));
	}

	emit flashingComplete(mFlashProcess.exitStatus() == QProcess::NormalExit);
}

bool NxtFlashTool::askToRun(QWidget *parent)
{
	return QMessageBox::question(parent, tr("The program has been uploaded")
			, tr("Do you want to run it?")) == QMessageBox::Yes;
}

void NxtFlashTool::readNxtFlashData()
{
	QStringList const output = QString(mFlashProcess.readAll()).split("\n", QString::SkipEmptyParts);

	foreach (QString const &error, output) {
		if (error == "NXT not found. Is it properly plugged in via USB?") {
			mErrorReporter->addError(tr("NXT not found. Check USB connection and make sure the robot is ON"));
		} else if (error == "NXT found, but not running in reset mode.") {
			mErrorReporter->addError(tr("NXT is not in reset mode. Please reset your NXT manually and try again"));
		} else if (error == "Firmware flash complete.") {
			mErrorReporter->addInformation(tr("Firmware flash complete!"));
		}
	}
}

void NxtFlashTool::uploadProgram(QFileInfo const &fileInfo)
{
	if (mIsUploading) {
		mErrorReporter->addInformation(tr("Uploading is already running"));
		return;
	}

	mIsUploading = true;
	mSource = fileInfo;

#ifdef Q_OS_WIN
	mUploadProcess.setWorkingDirectory(qApp->applicationDirPath() + "/nxt-tools/");
	mUploadProcess.start("cmd", QStringList() << "/c" << qApp->applicationDirPath() + "/nxt-tools/upload.bat " + fileInfo.baseName()
						 + " " + fileInfo.absolutePath());
#else
	Q_UNUSED(fileInfo)
	mUploadProcess.start("sh", QStringList() << qApp->applicationDirPath() + "/nxt-tools/upload.sh");
#endif

	mErrorReporter->addInformation(tr("Uploading program started. Please don't disconnect robot during the process"));
}

void NxtFlashTool::nxtUploadingFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus)

	mIsUploading = false;

	if (exitCode == 127) { // most likely wineconsole didn't start and generate files needed to proceed compilation
		mErrorReporter->addError(tr("Uploading failed. Make sure that X-server allows root to run GUI applications"));
	} else if (exitCode == 139) {
		mErrorReporter->addError(tr("QReal requires superuser privileges to flash NXT robot"));
	}

	emit uploadingComplete(mUploadState == done);
}

void NxtFlashTool::readNxtUploadData()
{
	QStringList const output = QString(mUploadProcess.readAll()).split("\n", QString::SkipEmptyParts);

	/* each command produces its own output, so thousands of 'em. using UploadState enum
	   to determine in which state we are (to show appropriate error if something goes wrong)
	*/

	foreach (QString const &error, output) {
		if (error.contains("Removing ")) {
			mUploadState = clean;
		} else if (error.contains("Compiling ")) {
			mUploadState = compile;
		} else if (error.contains("recipe for target") && error.contains("failed")) {
			mUploadState = compilationError;
		} else if (error.contains("Generating binary image file")) {
			mUploadState = link;
		} else if (error.contains("Executing NeXTTool to upload") && mUploadState != compilationError) {
			mUploadState = uploadStart;
		} else if (error.contains("_OSEK.rxe=") && mUploadState != compilationError) {
			mUploadState = flash;
		} else if (error.contains("NeXTTool is terminated")) {
			if (mUploadState == uploadStart) {
				mErrorReporter->addError(tr("Could not upload program. Make sure the robot is connected and ON"));
			} else if (mUploadState == flash) {
				mErrorReporter->addInformation(tr("Uploading completed successfully"));
				mUploadState = done;
			} else if (mUploadState == compilationError) {
				mErrorReporter->addError(tr("Compilation error occured. Please check your function blocks syntax. If you sure in their validness contact developers"));
			}
		} else if (error.contains("An unhandled exception occurred")) {
			mErrorReporter->addError(tr("QReal requires superuser privileges to upload programs on NXT robot"));
			break;
		}
	}
}

