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

#include <QtCore/QDirIterator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>
#include <qrkernel/settingsManager.h>
#include <nxtKit/communication/nxtCommandConstants.h>
#include <nxtKit/communication/usbRobotCommunicationThread.h>

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
	connect(&mCommunicator, &utils::robotCommunication::RobotCommunicationThreadInterface::messageArrived
			, this, [=](const QString &message) { mErrorReporter.addInformation(message); });
	connect(&mCommunicator, &utils::robotCommunication::RobotCommunicationThreadInterface::connected
			, this, [=](bool success, const QString &errorString)
	{
		if (!success) {
			mErrorReporter.addError(errorString);
		}
	});
}

bool NxtFlashTool::flashRobot()
{
	if (mIsFlashing) {
		mErrorReporter.addInformation(tr("Robot is already being flashed"));
		return false;
	}

	const QFileInfo firmwareBinary = findLatestFirmware();
	if (!firmwareBinary.exists()) {
		mErrorReporter.addError(tr("Firmware file not found in nxt-tools directory."));
		return false;
	}

	auto const usbCommunicator = dynamic_cast<nxt::communication::UsbRobotCommunicationThread *>(&mCommunicator);
	if (!usbCommunicator) {
		mErrorReporter.addError(tr("Flashing robot is possible only by USB. Please switch to USB mode."));
		return false;
	}

	if (!usbCommunicator->connectFirmware()) {
		return false;
	}

	mIsFlashing = true;
	mErrorReporter.addInformation(tr("Firmware flash started. Please don't disconnect robot during the process"));
	return true;
}

bool NxtFlashTool::runProgram(const QFileInfo &fileInfo)
{
	if (!mCommunicator.connect()) {
		return false;
	}

	const QString programNameOnBrick = nxtProgramName(fileInfo);
	if (programNameOnBrick.isEmpty()) {
		return false;
	}

	mSource = fileInfo;
	QByteArray responce;
	mCommunicator.send(fileNameTelegram(enums::telegramType::directCommandResponseRequired
			, enums::commandCode::STARTPROGRAM, programNameOnBrick, 0), 5, responce);

	return responce.length() >= 5 && static_cast<qint8>(responce[4]) == 0;
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

//void NxtFlashTool::readNxtFlashData()
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
	mCompileState = idle;
	mSource = fileInfo;

#ifdef Q_OS_WIN
	mCompileProcess.setWorkingDirectory(path());
	mCompileProcess.start("cmd", { "/c", path("compile.bat")
						+ " " + fileInfo.completeBaseName()
						+ " " + fileInfo.absolutePath() });
#else
	Q_UNUSED(fileInfo)
	mCompileProcess.start("sh", { path("compile.sh") });
#endif

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

	if (mCompileState != done) {
		emit uploadingComplete(false);
	}
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

	for (const QString &message : output) {
		if (message.contains("Removing ")) {
			mCompileState = qMax(clean, mCompileState);
		}

		if (message.contains("Compiling ")) {
			mCompileState = qMax(compile, mCompileState);
		}

		if (message.contains("recipe for target") && message.contains("failed")) {
			mCompileState = qMax(compilationError, mCompileState);
		}

		if (message.contains("Generating binary image file")) {
			mCompileState = qMax(link, mCompileState);
		}

		if (message.contains("Compiling NXT program done")) {
			if (mCompileState == link) {
				mCompileState = done;
				uploadToBrick(mSource);
			} else if (mCompileState == compilationError) {
				mErrorReporter.addError(tr("Compilation error occured. Please check your function blocks syntax. "
						"If you sure in their validness contact developers"));
			} else {
				mErrorReporter.addError(tr("Could not upload program. Make sure the robot is connected and ON"));
			}
		}

		if (message.contains("An unhandled exception occurred")) {
			mErrorReporter.addError(tr("QReal requires superuser privileges to upload programs on NXT robot"));
			break;
		}
	}
}

QString NxtFlashTool::path(const QString &file) const
{
	return QDir::toNativeSeparators(PlatformInfo::invariantSettingsPath("pathToNxtTools") + "/" + file);
}

QString NxtFlashTool::nxtProgramName(const QFileInfo &srcFile) const
{
	// NXT crops file name to 15 letters, so doing same here...
	return QString("%1.rxe").arg(srcFile.completeBaseName().mid(0, 15));
}

bool NxtFlashTool::uploadToBrick(const QFileInfo &fileOnHost)
{
	const QString executableOnHost = fileOnHost.absolutePath() + "/" + fileOnHost.completeBaseName() + ".rxe";
	if (!mCommunicator.connect()) {
		emit uploadingComplete(false);
		return false;
	}

	QFile file(executableOnHost);
	QDataStream stream(&file);
	if (!file.open(QFile::ReadOnly)) {
		mErrorReporter.addError(tr("Could not find %1. Check your program was compiled and try again.")
				.arg(executableOnHost));
		emit uploadingComplete(false);
		return false;
	}

	const QString fileOnBrick = nxtProgramName(fileOnHost);
	quint8 handle;

	if (!deleteFileFromBrick(fileOnBrick)) {
		mErrorReporter.addError(tr("Could not delete old file. Make sure the robot is connected, turned on."));
		emit uploadingComplete(false);
		return false;
	}

	if (!createFileOnBrick(fileOnBrick, file.size(), handle)) {
		mErrorReporter.addError(tr("Could not upload program. Make sure the robot is connected, turned on and has "\
				"enough free memory."));
		emit uploadingComplete(false);
		return false;
	}

	if (!downloadStreamToBrick(handle, stream, file.size())) {
		mErrorReporter.addError("Could not write file data to a robot.  Make sure robot is connected and turned on.");
		emit uploadingComplete(false);
		return false;
	}

	if (!closeFileOnBrick(handle)) {
		mErrorReporter.addError(tr("Could not close file on brick. Probably connection to NXT lost at "\
				"the last stage of uploading"));
		emit uploadingComplete(false);
		return false;
	}

	mErrorReporter.addInformation(tr("Uploading completed successfully"));
	emit uploadingComplete(true);
	return true;
}

QFileInfo NxtFlashTool::findLatestFirmware() const
{
	QDirIterator iterator(path("nexttool"), QDirIterator::FollowSymlinks | QDirIterator::Subdirectories);
	QString latestFirmware;
	while (iterator.hasNext()) {
		const QString currentFirmware = iterator.next();
		qDebug() << currentFirmware;
		if (currentFirmware.endsWith(".rfw") && currentFirmware > latestFirmware) {
			qDebug() << "making" << currentFirmware << "latest";
			latestFirmware = currentFirmware;
		}
	}

	return latestFirmware.isEmpty() ? QFileInfo() : QFileInfo(latestFirmware);
}

bool NxtFlashTool::deleteFileFromBrick(const QString &fileOnBrick)
{
	if (!mCommunicator.connect()) {
		return false;
	}

	const QByteArray buffer = fileNameTelegram(enums::telegramType::systemCommandResponseRequired
			, enums::systemCommandCode::deleteCommand, fileOnBrick, 0);

	QByteArray responce;
	mCommunicator.send(buffer, 25, responce);

	return !responce.isEmpty();
}

bool NxtFlashTool::createFileOnBrick(const QString &fileOnBrick, int fileSize, quint8 &handle)
{
	if (!mCommunicator.connect()) {
		return false;
	}


	enums::systemCommandCode::SystemCommandCodeEnum fileMode = enums::systemCommandCode::openWrite;
	if (fileOnBrick.endsWith(".rxe") || fileOnBrick.endsWith(".sys") || fileOnBrick.endsWith(".rtm")
			|| fileOnBrick.endsWith(".rpg") || fileOnBrick.endsWith(".ric") || fileOnBrick.endsWith(".rbm"))
	{
		// Then we uploading executable or graphics file, should use linear strategy
		fileMode = enums::systemCommandCode::openWriteLinear;
	} else if (fileOnBrick.endsWith(".rdt")) {
		// Then we uploading data file, should use data strategy
		fileMode = enums::systemCommandCode::openWriteData;
	}

	const QByteArray buffer = fileNameTelegram(enums::telegramType::systemCommandResponseRequired
			, fileMode, fileOnBrick, fileSize);
	QByteArray responce;
	mCommunicator.send(buffer, 6, responce);
	if (responce.length() >= 6) {
		if (responce[4]) {
			QLOG_ERROR() << "Lego NXT answered with error code" << responce[4];
		} else {
			handle = responce[5];
		}

		return true;
	}

	handle = UINT8_MAX;
	return false;
}

bool NxtFlashTool::downloadStreamToBrick(quint8 handle, QDataStream &inputStream, int fileSize)
{
	if (!mCommunicator.connect()) {
		return false;
	}

	int index = 0;
	while (index < fileSize) {
		const int currentDataSize = qMin(maxMessageSize, fileSize - index);
		index += currentDataSize;
		QByteArray buffer(currentDataSize + 5, '\0');
		buffer[0] = currentDataSize + 3;  // Bluetooth header
		buffer[1] = '\0';
		buffer[2] = enums::telegramType::systemCommandResponseRequired;  // NXT header
		buffer[3] = enums::systemCommandCode::write;
		buffer[4] = handle;
		for (int i = 0; i < currentDataSize; ++i) {
			qint8 nextByte;
			inputStream >> nextByte;
			buffer[5 + i] = nextByte;
		}

		QByteArray responce;
		mCommunicator.send(buffer, 8, responce);
		if (responce.isEmpty()) {
			return false;
		}
	}

	return true;
}

bool NxtFlashTool::closeFileOnBrick(quint8 handle)
{
	// 2 bytes NXT header + 1 byte for file handle
	const quint8 commandLength = 3;
	QByteArray result(commandLength + 2, '\0');

	int index = 0;
	result[index++] = commandLength;  // Bluetooth header
	result[index++] = '\0';

	result[index++] = enums::telegramType::systemCommandResponseRequired;
	result[index++] = enums::systemCommandCode::close;

	result[index++] = handle;

	QByteArray responce;
	mCommunicator.send(result, 6, responce);

	return (responce.length() >= 6) && static_cast<char>(responce[4]) == 0;
}

QByteArray NxtFlashTool::fileNameTelegram(quint8 commandType, quint8 command
		, const QString &fileName, int fileSize) const
{
	// 2 bytes NXT header +
	// 19 bytes for filename (maximally 15 for base name + '.' + extension) + null-character +
	// 4 bytes for size if non-zero.
	const quint8 commandLength = fileSize ? 26 : 22;
	QByteArray result(commandLength + 2, '\0');

	int index = 0;
	result[index++] = commandLength;  // Bluetooth header
	result[index++] = '\0';

	result[index++] = commandType;
	result[index++] = command;

	// Raw filename goes next.
	for (int i = 0; i < 19; ++i) {
		if (i < fileName.length()) {
			result[index] = fileName[i].toLatin1();
		}

		++index;
	}

	result[index++] = '\0';  // Null-terminating character.

	// 4 bytes for size
	if (fileSize > 0) {
		result[index++] = static_cast<qint8>(fileSize & 0xFF);
		result[index++] = static_cast<qint8>((fileSize >> 8) & 0xFF);
		result[index++] = static_cast<qint8>((fileSize >> 16) & 0xFF);
		result[index++] = static_cast<qint8>((fileSize >> 24) & 0xFF);
	}

	return result;
}
