/* Copyright 2012-2016 QReal Research Group
 *
 * Acknowledgements to David Anderson for his libnxt masterpiece that influenced our firmware flashing code a lot.
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

#include <QtCore/QFuture>
#include <QtCore/QDirIterator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>
#include <qrkernel/settingsManager.h>
#include <thirdparty/runExtensions.h>
#include <nxtKit/communication/nxtCommandConstants.h>
#include <nxtKit/communication/usbRobotCommunicationThread.h>

using namespace nxt;
using namespace qReal;

static const quint32 lockRegionCommand = 0x2;
static const quint32 unlockRegionCommand = 0x4;

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
	connect(&mCompileProcess, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished)
			, this, &NxtFlashTool::nxtCompilationFinished);

	connect(&mCommunicator, &utils::robotCommunication::RobotCommunicationThreadInterface::errorOccured
			, this, &NxtFlashTool::error);
	connect(&mCommunicator, &utils::robotCommunication::RobotCommunicationThreadInterface::messageArrived
			, this, &NxtFlashTool::information);
	connect(&mCommunicator, &utils::robotCommunication::RobotCommunicationThreadInterface::connected
			, this, [this](bool success, const QString &errorString)
	{
		if (!success) {
			error(errorString);
		}
	});
}

bool NxtFlashTool::flashRobot()
{
	if (mIsFlashing) {
		information(tr("Robot is already being flashed"));
		return false;
	}

	QLOG_INFO() << "Flashing NXT robot...";
	const QFileInfo firmwareBinaryName = findLatestFirmware();
	if (!firmwareBinaryName.exists()) {
		QLOG_ERROR() << "Could not find firmware binary";
		error(tr("Firmware file not found in nxt-tools directory."));
		return false;
	}

	auto const usbCommunicator = dynamic_cast<nxt::communication::UsbRobotCommunicationThread *>(&mCommunicator);
	if (!usbCommunicator) {
		QLOG_ERROR() << "Attempted to flash robot in bluetooth mode";
		error(tr("Flashing robot is possible only by USB. Please switch to USB mode."));
		return false;
	}

	if (!usbCommunicator->connectFirmware()) {
		QLOG_ERROR() << "Connection to NXT in firmware mode failed, see details above";
		return false;
	}

	mIsFlashing = true;
	information(tr("Firmware flash started. Please don't disconnect robot during the process"));

	// This lambda will be invoked in separete thread via QtConcurrent, progress will be reported.
	std::function<void(QFutureInterface<void> &)> flashProcess = [=](QFutureInterface<void> &future) {
		future.setProgressRange(0, 1150);  // preparation (50) + flashing 1024 + starting(76)
		future.setProgressValue(0);

		QFile firmwareBinary(firmwareBinaryName.absoluteFilePath());  // Will be closed when out of scope in destructor.
		if (!firmwareBinary.open(QFile::ReadOnly)) {
			QLOG_ERROR() << "Could not open" << firmwareBinaryName.absoluteFilePath()
					<< "for reading:" << firmwareBinary.errorString();
			error(tr("Could not open %1 for reading.").arg(firmwareBinaryName.absoluteFilePath()));
			mIsFlashing = false;
			return;
		}

		if (firmwareBinary.size() > 256 * 1024) {
			QLOG_ERROR() << "Firmware binary file size is" << firmwareBinary.size()
					<< "bytes which is too large for NXT";
			error(tr("Firmware file is too large to fit into NXT brick memory."));
			mIsFlashing = false;
			return;
		}

		QDataStream firmwareStream(&firmwareBinary);
		future.setProgressValue(25);

		if (!flashFirmwareStream(firmwareStream, future)) {
			QLOG_ERROR() << "Could not flash firmware into NXT brick. See details above";
			error(tr("Could not write firmware into NXT memory."));
			mIsFlashing = false;
			return;
		}

		if (!startNewFirmware()) {
			QLOG_ERROR() << "Could not jump to start new firmware";
			error(tr("Firmware successfully flashed into robot, but starting it failed."));
			mIsFlashing = false;
			return;
		}

		future.setProgressValue(future.progressMaximum());
		information(tr("Flashing process completed successfully."));
		QLOG_INFO() << "Firmware flashed successfully";
		usbCommunicator->disconnect();
		mIsFlashing = false;
		return;
	};

	mErrorReporter.reportOperation(QtConcurrent::run(flashProcess), tr("Flashing NXT brick..."));
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
	if (!mCommunicator.send(fileNameTelegram(enums::telegramType::directCommandResponseRequired
			, enums::commandCode::STARTPROGRAM, programNameOnBrick, 0), 5, responce))
	{
		return false;
	}

	return responce.length() >= 5 && static_cast<qint8>(responce[4]) == 0;
}

bool NxtFlashTool::runLastProgram()
{
	return mSource != QFileInfo() && runProgram(mSource);
}

void NxtFlashTool::information(const QString &message)
{
	if (QThread::currentThread() == qApp->thread()) {
		// We are currently in GUI thread, simply calling method of error reporter.
		mErrorReporter.addInformation(message);
	} else if (QObject * const object = dynamic_cast<QObject *>(&mErrorReporter)) {
		// We are not in GUI thread, calling of error reporter asynchroniously.
		QMetaObject::invokeMethod(object, "addInformation", Q_ARG(QString, message));
	} else {
		QLOG_ERROR() << "Cannot display" << message << "in non-gui thread cause mErrorReporter is not QObject";
	}
}

void NxtFlashTool::error(const QString &message)
{
	if (QThread::currentThread() == qApp->thread()) {
		// We are currently in GUI thread, simply calling method of error reporter.
		mErrorReporter.addError(message);
	} else if (QObject * const object = dynamic_cast<QObject *>(&mErrorReporter)) {
		// We are not in GUI thread, calling of error reporter asynchroniously.
		QMetaObject::invokeMethod(object, "addError", Q_ARG(QString, message));
	} else {
		QLOG_ERROR() << "Cannot display" << message << "in non-gui thread cause mErrorReporter is not QObject";
	}
}

bool NxtFlashTool::askToRun(QWidget *parent)
{
	return QMessageBox::question(parent, tr("The program has been uploaded")
			, tr("Do you want to run it?")) == QMessageBox::Yes;
}

bool NxtFlashTool::uploadProgram(const QFileInfo &fileInfo)
{
	if (mIsUploading) {
		information(tr("Uploading is already running"));
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

	information(tr("Uploading program started. Please don't disconnect robot during the process"));
	return true;
}

void NxtFlashTool::nxtCompilationFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus)

	mIsUploading = false;

	if (exitCode == 127) { // most likely wineconsole didn't start and generate files needed to proceed compilation
		error(tr("Uploading failed. Make sure that X-server allows root to run GUI applications"));
	} else if (exitCode == 139) {
		error(tr("You need to have superuser privileges to flash NXT robot"));
	}

	if (mCompileState != done) {
		emit uploadingComplete(false);
	}
}

void NxtFlashTool::readNxtCompileData()
{
	const QStringList output = QString(mCompileProcess.readAll()).split("\n", QString::SkipEmptyParts);
	const QString processError = mCompileProcess.readAllStandardError();
	QLOG_INFO() << "NXT flash tool:" << output;
	if (!processError.isEmpty()) {
		QLOG_ERROR() << "NXT flash tool: error:" << processError;
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
				error(tr("Compilation error occured. Please check your function blocks syntax. "
						"If you sure in their validness contact developers"));
			} else {
				error(tr("Could not upload program. Make sure the robot is connected and ON"));
			}
		}

		if (message.contains("An unhandled exception occurred")) {
			error(tr("QReal requires superuser privileges to upload programs on NXT robot"));
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

bool NxtFlashTool::flashFirmwareStream(QDataStream &firmware, QFutureInterface<void> &progressTracker)
{
	if (!prepareFlashing()) {
		QLOG_ERROR() << "Could not prepare for flashing, see details above";
		return false;
	}

	progressTracker.setProgressValue(50);
	QLOG_INFO() << "Flashing firmware stream...";
	const int chunkSize = 256;
	for (int i = 0; i < 1024; ++i) {
		QByteArray buffer(256, '\0');
		const int bytesRead = firmware.readRawData(buffer.data(), chunkSize);
		if (bytesRead < 0) {
			const QString errorString = firmware.device()->errorString();
			QLOG_ERROR() << "Error in reading bytes in firmware stream from" << i * 256 << "to" << ((i+1) * 256 - 1)
					<< ":" << errorString;
			error(tr("Error in reading from firmware file: %1").arg(errorString));
			return false;
		}

		if (!flashOneBlock(i, buffer)) {
			QLOG_ERROR() << "Flashing block" << i << "failed!";
			return false;
		}

		progressTracker.setProgressValue(51 + i);
		if (bytesRead < chunkSize) {
			break;
		}
	}

	return waitTillFlashingIsReady();
}

bool NxtFlashTool::flashOneBlock(int orderNumber, const QByteArray &block)
{
	// Setting the target block number...
	if (!write32InSambaMode(0x202300, orderNumber)) {
		QLOG_ERROR() << "Failed to set block order number for block" << orderNumber;
		return false;
	}

	// Sending block data...
	if (!writeBufferInSambaMode(0x202100, block)) {
		QLOG_ERROR() << "Failed to flash data for block" << orderNumber;
		return false;
	}

	// And finally activating flash writing routine.
	if (!jumpInSambaMode(0x202000)) {
		QLOG_ERROR() << "Failed to activate flashing routing for block" << orderNumber;
		return false;
	}

	return true;
}

bool NxtFlashTool::startNewFirmware()
{
	return jumpInSambaMode(0x00100000);
}

bool NxtFlashTool::prepareFlashing()
{
	// Putting clock into PPL/2 mode
	if (!write32InSambaMode(0xFFFFFC30, 0x7)) {
		QLOG_ERROR() << "Could not put clock into PPL/2 mode";
		return false;
	}

	if (!unlockFlashChip()) {
		QLOG_ERROR() << "Could not unlock flash chip for writing data into it";
		return false;
	}

	// We need a help of special flashin routine on the NXT brick side. Flashing it into NXT memory first...
	QFile flashingRoutine(":/nxt/osek/flash.bin");  // Will be closed when out of scope in destructor.
	if (!flashingRoutine.open(QFile::ReadOnly)) {
		QLOG_ERROR() << "Could not open" << flashingRoutine.fileName() << "for reading, that's strange! Error:"
				<< flashingRoutine.errorString();
		return false;
	}

	const QByteArray flashingRoutineData = flashingRoutine.readAll();
	if (flashingRoutineData.isEmpty()) {
		QLOG_ERROR() << "Flashing routine is empty, go and buy a new brain";
		return false;
	}

	if (!writeBufferInSambaMode(0x202000, flashingRoutineData)) {
		QLOG_ERROR() << "Could not write flashing routine, giving up!";
		return false;
	}

	return true;
}

bool NxtFlashTool::jumpInSambaMode(quint32 address)
{
	const QString command = QString("  G%1#").arg(address, 8, 16, QLatin1Char('0'));
	QByteArray responce;
	return mCommunicator.send(command.toLatin1(), 0, responce);
}

QByteArray NxtFlashTool::sambaCommandTeleram(quint32 address, char type, quint32 data) const
{
	return (QString("  ") + type + QString("%1,%2#").arg(address, 8, 16, QLatin1Char('0'))
			.arg(data, 8, 16, QLatin1Char('0')).toUpper()).toLatin1();
}

bool NxtFlashTool::write8InSambaMode(quint32 address, quint8 data)
{
	return writeIntegerInSambaMode(address, 'O', data);
}

bool NxtFlashTool::write16InSambaMode(quint32 address, quint16 data)
{
	return writeIntegerInSambaMode(address, 'H', data);
}

bool NxtFlashTool::write32InSambaMode(quint32 address, quint32 data)
{
	return writeIntegerInSambaMode(address, 'W', data);
}

bool NxtFlashTool::writeIntegerInSambaMode(quint32 address, char type, quint32 data)
{
	const QByteArray command = sambaCommandTeleram(address, type, data);
	QByteArray responce;
	return mCommunicator.send(command, 0, responce);
}

bool NxtFlashTool::writeBufferInSambaMode(quint32 address, const QByteArray &data)
{
	const QByteArray command = sambaCommandTeleram(address, 'S', data.length());
	QByteArray responce;
	return mCommunicator.send(command, 0, responce) && mCommunicator.send(QByteArray("  ") + data, 0, responce);
}

bool NxtFlashTool::read8InSambaMode(quint32 address, quint8 &data)
{
	quint32 bigData;
	if (!readIntegerInSambaMode(address, 'o', 1, bigData)) {
		return false;
	}

	data = static_cast<quint8>(bigData);
	return true;
}

bool NxtFlashTool::read16InSambaMode(quint32 address, quint16 &data)
{
	quint32 bigData;
	if (!readIntegerInSambaMode(address, 'h', 2, bigData)) {
		return false;
	}

	data = static_cast<quint16>(bigData);
	return true;
}

bool NxtFlashTool::read32InSambaMode(quint32 address, quint32 &data)
{
	return readIntegerInSambaMode(address, 'w', 4, data);
}

bool NxtFlashTool::readIntegerInSambaMode(quint32 address, char type, int length, quint32 &data)
{
	const QByteArray readingCommand = sambaCommandTeleram(address, type, length);
	QByteArray responce;
	if (!mCommunicator.send(readingCommand, length + 2, responce) || responce.isEmpty()) {
		return false;
	}

	const char *responceBytes = responce.constData();
	/// @todo: Consider big-endian case?
	data = *reinterpret_cast<const quint32 *>(responceBytes + 2);

	return true;
}

bool NxtFlashTool::waitTillFlashingIsReady()
{
	quint32 status = 0;
	do {
		if (!read32InSambaMode(0xFFFFFF68, status)) {
			QLOG_ERROR() << "Reading error while waiting for flashing is ready!";
			return false;
		}
	} while (!(status & 0x1));
	return true;
}

bool NxtFlashTool::unlockFlashChip()
{
	for (int i = 0; i < 16; ++i) {
		if (!lockOrUnlockRegion(i, false)) {
			return false;
		}
	}

	return true;
}

bool NxtFlashTool::lockOrUnlockRegion(int regionNumber, bool lock)
{
	const quint32 lockCommandNumber = lock ? lockRegionCommand : unlockRegionCommand;
	const quint32 command = (0x5A000000 | ((64 * regionNumber) << 8)) + lockCommandNumber;

	if (!waitTillFlashingIsReady()) {
		return false;
	}

	// Flash mode register: FCMN 0x5, FWS 0x1
	// Flash command register: KEY 0x5A, FCMD = clear-lock-bit (0x4)
	// Flash mode register: FCMN 0x34, FWS 0x1
	bool result = true;
	result &= write32InSambaMode(0xFFFFFF60, 0x00050100);
	result &= write32InSambaMode(0xFFFFFF64, command);
	result &= write32InSambaMode(0xFFFFFF60, 0x00340100);
	if (!result) {
		QLOG_ERROR() << (lock ? "Locking" : "Unlocking") << "region" << regionNumber << "failed!";
	}

	return result;
}

bool NxtFlashTool::uploadToBrick(const QFileInfo &fileOnHost)
{
	const QString executableOnHost = fileOnHost.absolutePath() + "/" + fileOnHost.completeBaseName() + ".rxe";
	if (!mCommunicator.connect()) {
		emit uploadingComplete(false);
		return false;
	}

	// Will be closed when out of scope in destructor.
	QFile file(executableOnHost);
	QDataStream stream(&file);
	if (!file.open(QFile::ReadOnly)) {
		error(tr("Could not find %1. Check your program was compiled and try again.").arg(executableOnHost));
		emit uploadingComplete(false);
		return false;
	}

	const QString fileOnBrick = nxtProgramName(fileOnHost);
	quint8 handle;

	if (!deleteFileFromBrick(fileOnBrick)) {
		error(tr("Could not delete old file. Make sure the robot is connected, turned on."));
		emit uploadingComplete(false);
		return false;
	}

	if (!createFileOnBrick(fileOnBrick, file.size(), handle)) {
		error(tr("Could not upload program. Make sure the robot is connected, turned on and has "\
				"enough free memory."));
		emit uploadingComplete(false);
		return false;
	}

	if (!downloadStreamToBrick(handle, stream, file.size())) {
		error("Could not write file data to a robot.  Make sure robot is connected and turned on.");
		emit uploadingComplete(false);
		return false;
	}

	if (!closeFileOnBrick(handle)) {
		error(tr("Could not close file on brick. Probably connection to NXT lost at "\
				"the last stage of uploading"));
		emit uploadingComplete(false);
		return false;
	}

	information(tr("Uploading completed successfully"));
	emit uploadingComplete(true);
	return true;
}

QFileInfo NxtFlashTool::findLatestFirmware() const
{
	QDirIterator iterator(path("nexttool"), QDirIterator::FollowSymlinks | QDirIterator::Subdirectories);
	QString latestFirmware;
	while (iterator.hasNext()) {
		const QString currentFirmware = iterator.next();
		if (currentFirmware.endsWith(".rfw") && currentFirmware > latestFirmware) {
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
	return mCommunicator.send(buffer, 25, responce);
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
	if (!mCommunicator.send(buffer, 6, responce)) {
		return false;
	}

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
		if (!mCommunicator.send(buffer, 8, responce)) {
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
