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

	~Reporter();

public slots:
	/// Reports informational message reported to user during the interpretation process.
	void addInformation(const QString &message);

	/// Reports the error message reported to user during the interpretation process.
	void addError(const QString &message);

	/// Writes an information abount the new trajectory point into the given in constructor file.
	void newTrajectoryPoint(int timestamp, const QPointF &position, qreal rotation);

	/// Writes an information about all messages shown to user during the interpretation process into the specified
	/// in constructor file in XML format.
	void reportMessages();

private:
	QList<QPair<Level, QString>> mMessages;
	const QScopedPointer<utils::OutFile> mMessagesFile;
	const QScopedPointer<utils::OutFile> mTrajectoryFile;
};

}
