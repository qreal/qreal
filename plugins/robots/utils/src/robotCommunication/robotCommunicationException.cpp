#include "include/utils/robotCommunication/robotCommunicationException.h"

using namespace utils::robotCommunication;

RobotCommunicationException::RobotCommunicationException(QString const &message)
	: qReal::Exception(message)
{
}
