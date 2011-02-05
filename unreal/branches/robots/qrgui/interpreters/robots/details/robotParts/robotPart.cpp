#include "robotPart.h"

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

RobotPart::RobotPart(RobotCommunicationInterface * const robotCommunicationInterface)
	: mRobotCommunicationInterface(robotCommunicationInterface)
{
	connect(robotCommunicationInterface, SIGNAL(response(QObject*,QByteArray))
			, this, SLOT(readingDone(QObject*,QByteArray)));
}

void RobotPart::readingDone(QObject *addressee, QByteArray const &reading)
{
	if (addressee != this)
		return;
	processResponse(reading);
}

void RobotPart::processResponse(QByteArray const &reading)
{
	Q_UNUSED(reading);
}
