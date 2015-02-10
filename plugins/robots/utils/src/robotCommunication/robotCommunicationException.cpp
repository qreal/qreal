#include "include/utils/robotCommunication/robotCommunicationException.h"

using namespace utils::robotCommunication;

RobotCommunicationException::RobotCommunicationException(const QString &message)
	: qReal::Exception(message)
{
}
