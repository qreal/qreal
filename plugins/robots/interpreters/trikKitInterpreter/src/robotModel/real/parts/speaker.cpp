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
	qDebug() << "Speaker dfgfg" << filePath << "\n";
	QString const pathToCommand = ":/trik/templates/playTone.t";
	QString directCommand = utils::InFile::readAll(pathToCommand);
	directCommand = directCommand.replace("@@FILENAME@@", filePath);
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
}
