#pragma once

#include <QtCore/QObject>
#include <stdio.h>

//!
//! @brief The Communicator class
//! provides simple IO methods with saving information
class Communicator : public QObject
{
	Q_OBJECT
public:
	explicit Communicator(QObject *parent = 0);

	//! Reads and saves path to main app (must be first input)
	void readProgramPath();
	//! Sends 'terminate' message. Main app must be terminated immediatly in this case
	void writeQuitMessage();
	//! Sends 'resume' message. No action required
	void writeResumeMessage();
	void writeHelpMessage();

	//! @return saved path to main application
	QString parentAppPath() const;
protected:
	QString mParentAppPath;
};

