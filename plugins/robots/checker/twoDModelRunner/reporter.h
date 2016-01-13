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

#pragma once

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

namespace utils {
class OutFile;
}

namespace twoDModel {

/// Represents the type of the message showed to user.
enum class Level
{
	/// Informational message, does not contain information about errors or smth.
	information = 0
	/// Error message if something went wrong.
	, error
};

/// Collects information about the interpretation process and writes it into the given file as JSON report.
class Reporter : public QObject
{
public:
	/// Constructor.
	/// @param messagesFile If non-empty the information about all error reporter messages will be stored there in JSON.
	/// In contrast to trajectory the info about messages will be accumulated and written in one moment after
	/// the interpretation ends.
	/// @param trajectoryFile If non-empty the information about robot`s movement will be stored there
	/// during the interpetation (so the factical data write will not be performed in one moment, it will be written
	/// in chunks, each chunk with the new robot transition).
	Reporter(const QString &messagesFile, const QString &trajectoryFile);

	~Reporter() override;

	/// Returns true if last reported to user message was an error.
	bool lastMessageIsError();

public slots:
	/// Reports informational message reported to user during the interpretation process.
	void addInformation(const QString &message);

	/// Reports the error message reported to user during the interpretation process.
	void addError(const QString &message);

	/// Resets inner state for new interpretation instance.
	void onInterpretationStart();

	/// Finalizes reports when interpretation has finished.
	void onInterpretationEnd();

	/// Writes an information abount the new trajectory point into the given in constructor file.
	/// @param robotId The id of the moved robot.
	/// @param timestamp Count of milliseconds passed from the interpretation start when the event happened.
	/// @param timestamp The position of the robot in scene coordinates.
	/// @param rotation The rotation angle of the robot in degrees.
	void newTrajectoryPoint(const QString &robotId, int timestamp, const QPointF &position, qreal rotation);

	/// Writes JSON object representing modification of some robot`s device property into the trajectory stream.
	/// @param robotId The id of the robot owning the device.
	/// @param timestamp Count of milliseconds passed from the interpretation start when the event happened.
	/// @param deviceType The type name of the device obtained by DeviceInfo::name().
	/// @param devicePort The name of the port the device is plugged into, obtained by PortInfo::name().
	/// @param property The name of the modified Q_PROPERTY.
	/// @param value The new value of the property.
	void newDeviceState(const QString &robotId, int timestamp, const QString &deviceType
			, const QString &devicePort, const QString &property, const QVariant &value);

	/// Writes an information about all messages shown to user during the interpretation process into the specified
	/// in constructor file in XML format.
	void reportMessages();

private:
	QJsonValue variantToJson(const QVariant &value) const;
	void report(const QString &message, const QScopedPointer<utils::OutFile> &file);

	QList<QPair<Level, QString>> mMessages;
	const QScopedPointer<utils::OutFile> mMessagesFile;
	const QScopedPointer<utils::OutFile> mTrajectoryFile;
	bool mFirstMessage;
};

}
