#include "robotCommunicationException.h"

using namespace robotsInterpreter::robotCommunication;

RobotCommunicationException::RobotCommunicationException(QString const &message)
	: Exception(message)
{
}
