#pragma once

#include <QtCore/QObject>
#include <stdio.h>

namespace qrUpdater {

/// Provides simple IO methods with saving information.
class Communicator : public QObject
{
	Q_OBJECT

public:
	explicit Communicator(QObject *parent = 0);

	/// Reads and saves path to main app (must be first input).
	void readProgramPath();

	/// Sends 'terminate' message. Main app must be terminated immediatly in this case.
	void writeQuitMessage();

	/// Sends 'resume' message. No action required.
	void writeResumeMessage();

	/// Prints to console given @arg message
	void writeCustomMessage(QString const &message);

	/// @return saved path to main application
	QString parentAppPath() const;

private:
	QString mParentAppPath;
};

}
