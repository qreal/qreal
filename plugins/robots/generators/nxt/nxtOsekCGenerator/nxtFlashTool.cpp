/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "nxtFlashTool.h"

#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtWidgets/QMessageBox>

using namespace nxt;
using namespace qReal;

NxtFlashTool::NxtFlashTool(qReal::ErrorReporterInterface &errorReporter
		, utils::robotCommunication::RobotCommunicationThreadInterface &communicator)
	: mErrorReporter(errorReporter)
	, mCommunicator(communicator)
	, mIsFlashing(false)
	, mIsUploading(false)
	, mCompileState(done)
{
	QProcessEnvironment environment(QProcessEnvironment::systemEnvironment());
	QString path = this->path();
	environment.insert("NXT_TOOLS_DIR", QString(path).replace("\\", "/"));
	environment.insert("NXT_TOOLS_DIR_POSIX", path.remove(1, 1).prepend("/cygdrive/").replace("\\", "/"));
	environment.insert("DISPLAY", ":0.0");
	mCompileProcess.setProcessEnvironment(environment);

	connect(&mCompileProcess, &QProcess::readyRead, this, &NxtFlashTool::readNxtCompileData);
	connect(&mCompileProcess, static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::error)
			, this, &NxtFlashTool::error);
	connect(&mCompileProcess, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished)
			, this, &NxtFlashTool::nxtCompilationFinished);

	connect(&mCommunicator, &utils::robotCommunication::RobotCommunicationThreadInterface::errorOccured
			, this, [=](const QString &message) { mErrorReporter.addError(message); });
}

bool NxtFlashTool::flashRobot()
{
	if (mIsFlashing) {
		mErrorReporter.addInformation(tr("Robot is already being flashed"));
		return false;
	}

	mIsFlashing = true;

#ifdef Q_OS_WIN
//	mFlashProcess.setEnvironment(QProcess::systemEnvironment());
//	mFlashProcess.setWorkingDirectory(path("nexttool"));
//	mFlashProcess.start("cmd", { "/c", path("flash.bat") });
#else
//	mFlashProcess.start("sh", { path("flash.sh") });
#endif

	mErrorReporter.addInformation(tr("Firmware flash started. Please don't disconnect robot during the process"));
	return true;
}

bool NxtFlashTool::runProgram(const QFileInfo &fileInfo)
{
	if (!mCommunicator.connect()) {
		return false;
	}

	mSource = fileInfo;
	return true;
//	mRunProcess.setEnvironment(QProcess::systemEnvironment());
//	mRunProcess.setWorkingDirectory(path());
//	mRunProcess.start("cmd", { "/c", path("nexttool/NexTTool.exe") + " /COM=usb -run="
//			// NXT crops file name to 15 letters, so doing same here...
//			+ QString("%1.rxe").arg(mSource.completeBaseName().mid(0, 15)) });
}

bool NxtFlashTool::runLastProgram()
{
	return mSource != QFileInfo() && runProgram(mSource);
}

void NxtFlashTool::error(QProcess::ProcessError error)
{
	Q_UNUSED(error)
	mErrorReporter.addInformation(tr("Some error occured. Make sure you are running QReal with superuser privileges"));
}

void NxtFlashTool::nxtFlashingFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus)

	mIsFlashing = false;

	if (exitCode == 0) {
		mErrorReporter.addInformation(tr("Flashing process completed."));
	} else if (exitCode == 127) {
		mErrorReporter.addError(tr("flash.sh not found. Make sure it is present in QReal installation directory"));
	} else if (exitCode == 139) {
		mErrorReporter.addError(tr("QReal requires superuser privileges to flash NXT robot"));
	}

//	emit flashingComplete(mFlashProcess.exitStatus() == QProcess::NormalExit);
}

bool NxtFlashTool::askToRun(QWidget *parent)
{
	return QMessageBox::question(parent, tr("The program has been uploaded")
			, tr("Do you want to run it?")) == QMessageBox::Yes;
}

//void NxtFlashTool::4FlashData()
//{
//	const QStringList output = QString(mFlashProcess.readAll()).split("\n", QString::SkipEmptyParts);

//	for (const QString &error : output) {
//		if (error == "NXT not found. Is it properly plugged in via USB?") {
//			mErrorReporter->addError(tr("NXT not found. Check USB connection and make sure the robot is ON"));
//		} else if (error == "NXT found, but not running in reset mode.") {
//			mErrorReporter->addError(tr("NXT is not in reset mode. Please reset your NXT manually and try again"));
//		} else if (error == "Firmware flash complete.") {
//			mErrorReporter->addInformation(tr("Firmware flash complete!"));
//		}
//	}
//}

bool NxtFlashTool::uploadProgram(const QFileInfo &fileInfo)
{
	if (mIsUploading) {
		mErrorReporter.addInformation(tr("Uploading is already running"));
		return false;
	}

	mIsUploading = true;
	mSource = fileInfo;

//#ifdef Q_OS_WIN
//	mUploadProcess.setWorkingDirectory(path());
//	mUploadProcess.start("cmd", { "/c", path("upload.bat")
//						+ " " + fileInfo.completeBaseName()
//						+ " " + fileInfo.absolutePath() });
//#else
//	Q_UNUSED(fileInfo)
//	mUploadProcess.start("sh", { path("upload.sh") });
//#endif

	mErrorReporter.addInformation(tr("Uploading program started. Please don't disconnect robot during the process"));
	return true;
}

void NxtFlashTool::nxtCompilationFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus)

	mIsUploading = false;

	if (exitCode == 127) { // most likely wineconsole didn't start and generate files needed to proceed compilation
		mErrorReporter.addError(tr("Uploading failed. Make sure that X-server allows root to run GUI applications"));
	} else if (exitCode == 139) {
		mErrorReporter.addError(tr("QReal requires superuser privileges to flash NXT robot"));
	}

	emit uploadingComplete(mCompileState == done);
}

bool NxtFlashTool::uploadToBrick(const QString &program)
{
	return false;
}

void NxtFlashTool::readNxtCompileData()
{
	const QStringList output = QString(mCompileProcess.readAll()).split("\n", QString::SkipEmptyParts);
	const QString error = mCompileProcess.readAllStandardError();
	QLOG_INFO() << "NXT flash tool:" << output;
	if (!error.isEmpty()) {
		QLOG_ERROR() << "NXT flash tool: error:" << error;
	}

	/* each command produces its own output, so thousands of 'em. using UploadState enum
	   to determine in which state we are (to show appropriate error if something goes wrong)
	*/

	for (const QString &error : output) {
		if (error.contains("Removing ")) {
			mCompileState = clean;
		}

		if (error.contains("Compiling ")) {
			mCompileState = compile;
		}

		if (error.contains("recipe for target") && error.contains("failed")) {
			mCompileState = compilationError;
		}

		if (error.contains("Generating binary image file")) {
			mCompileState = link;
		}

		if (error.contains("Compiling NXT program done")) {
			if (mCompileState == link) {
				mErrorReporter.addInformation(tr("Uploading completed successfully"));
				mCompileState = done;
			} else if (mCompileState == compilationError) {
				mErrorReporter.addError(tr("Compilation error occured. Please check your function blocks syntax. "
						"If you sure in their validness contact developers"));
			} else {
				mErrorReporter.addError(tr("Could not upload program. Make sure the robot is connected and ON"));
			}
		}

		if (error.contains("An unhandled exception occurred")) {
			mErrorReporter.addError(tr("QReal requires superuser privileges to upload programs on NXT robot"));
			break;
		}
	}
}

const QString NxtFlashTool::path(const QString &file) const
{
	return QDir::toNativeSeparators(PlatformInfo::invariantSettingsPath("pathToNxtTools") + "/" + file);
}
