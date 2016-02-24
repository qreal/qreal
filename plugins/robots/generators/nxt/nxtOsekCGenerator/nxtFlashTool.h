/* Copyright 2012-2016 QReal Research Group
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

#pragma once

#include <QtCore/QProcess>
#include <QtCore/QFileInfo>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <utils/robotCommunication/robotCommunicationThreadInterface.h>

template<typename T>
class QFutureInterface;

namespace nxt {

/// Class that handles flashing NXT robot via USB.
class NxtFlashTool : public QObject
{
	Q_OBJECT

public:
	enum RunPolicy {
		Ask = 0
		, AlwaysRun
		, NeverRun
	};

	NxtFlashTool(qReal::ErrorReporterInterface &errorReporter
			, utils::robotCommunication::RobotCommunicationThreadInterface &communicator);

public slots:
	/// Searches for the firmware image in nxt-tools/nexttool directory and flashes it into NXT brick.
	/// NXT brick must be reseted before this method is called. If multiple images found in nxt-tools/nexttool
	/// then latest (greatest lexicographically) will be flashed. Flashing is performed through the raw libusb or
	/// qextserialport connection, fantom driver and NeXTTool are not required.
	bool flashRobot();

	/// Compiles and uploads program with the given source \a fileInfo into NXT brick. Makefile should be placed
	/// next to \a fileInfo. Executable .rxe file will be built with cygwin make tool. Flashing is performed through
	/// the raw libusb or qextserialport connection, fantom driver and NeXTTool are not required.
	bool uploadProgram(const QFileInfo &fileInfo);

	/// Starts program with compiled from the given source \a fileInfo on NXT brick. Communication is performed
	/// through the raw libusb or qextserialport connection, fantom driver and NeXTTool are not required.
	/// @note \a fileInfo base name will be used and will be cropped to 15 symbols cause NXT do that.
	bool runProgram(const QFileInfo &fileInfo);

	/// Starts last program with compiled and uploaded with uploadRobot on NXT brick. Communication is performed
	/// through the raw libusb or qextserialport connection, fantom driver and NeXTTool are not required.
	/// @note \a fileInfo base name will be used and will be cropped to 15 symbols cause NXT do that.
	bool runLastProgram();

	bool askToRun(QWidget *parent);

signals:
	void flashingComplete(bool success);
	void uploadingComplete(bool success);

private:
	enum CompileState {
		idle,
		clean,
		compile,
		compilationError,
		link,
		done
	};

	QString path(const QString &file = QString()) const;
	QString nxtProgramName(const QFileInfo &srcFile) const;

	void information(const QString &message);
	void error(const QString &message);

	//--------------------- Flashing section ----------------------//

	QFileInfo findLatestFirmware() const;
	bool flashFirmwareStream(QDataStream &firmware, QFutureInterface<void> &progressTracker);
	bool flashOneBlock(int orderNumber, const QByteArray &block);
	bool startNewFirmware();
	bool prepareFlashing();

	bool jumpInSambaMode(quint32 address);
	QByteArray sambaCommandTeleram(quint32 address, char type, quint32 data) const;

	bool write8InSambaMode(quint32 address, quint8 data);
	bool write16InSambaMode(quint32 address, quint16 data);
	bool write32InSambaMode(quint32 address, quint32 data);
	bool writeIntegerInSambaMode(quint32 address, char type, quint32 data);
	bool writeBufferInSambaMode(quint32 address, const QByteArray &data);

	bool read8InSambaMode(quint32 address, quint8 &data);
	bool read16InSambaMode(quint32 address, quint16 &data);
	bool read32InSambaMode(quint32 address, quint32 &data);
	bool readIntegerInSambaMode(quint32 address, char type, int length, quint32 &data);

	bool waitTillFlashingIsReady();
	bool unlockFlashChip();
	bool lockOrUnlockRegion(int regionNumber, bool lock);

	//--------------------- Uploading section ----------------------//

	bool deleteFileFromBrick(const QString &fileOnBrick);
	bool createFileOnBrick(const QString &fileOnBrick, int fileSize, quint8 &handle);
	bool downloadStreamToBrick(quint8 handle, QDataStream &stream, int fileSize);
	bool closeFileOnBrick(quint8 handle);
	bool uploadToBrick(const QFileInfo &fileOnHost);

	/// Creates telegram with direct or system command (specified by \a commandType) invoking \a command on a brick
	/// with \a fileName. \a size used to specify file size, if zero then it will not be packed into telegram.
	QByteArray fileNameTelegram(quint8 commandType, quint8 command, const QString &fileName, int size) const;

	void nxtCompilationFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void readNxtCompileData();

	qReal::ErrorReporterInterface &mErrorReporter;
	utils::robotCommunication::RobotCommunicationThreadInterface &mCommunicator;
	QProcess mCompileProcess;

	bool mIsFlashing;
	bool mIsUploading;

	QFileInfo mSource;

	CompileState mCompileState;
};

}
