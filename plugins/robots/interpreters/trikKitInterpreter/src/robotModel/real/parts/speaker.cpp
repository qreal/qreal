#include "speaker.h"
#include <utils/tcpRobotCommunicator.h>
#include <qrutils/inFile.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Speaker::Speaker(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikSpeaker(info, port)
{
}

void Speaker::play(QString const &filePath)
{
	QString const pathToCommand = ":/trik/templates/playTone.t";
	QString directCommand = utils::InFile::readAll(pathToCommand) +"brick.run();";
	directCommand = directCommand.replace("@@FILENAME@@", filePath);
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
}
