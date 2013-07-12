#include "robotCommunicationException.h"

using namespace qReal::interpreters::robots::details;

RobotCommunicationException::RobotCommunicationException(QString const &message)
	: Exception(message)
{
}
