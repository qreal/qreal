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

	/// Prints to console given @arg message
	void writeMessage(QString const &message);
};

}
