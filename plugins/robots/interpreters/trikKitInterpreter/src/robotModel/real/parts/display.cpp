#include "display.h"
#include <qrutils/inFile.h>
#include <utils/tcpRobotCommunicator.h>
#include <QColor>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Display::Display(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikDisplay(info, port)
{
}

void Display::drawSmile(bool sad)
{
	QString const pathToCommand = sad ? ":/trik/templates/drawing/sadSmile.t" : ":/trik/templates/drawing/smile.t";
	qDebug() << pathToCommand << "\n";
	QString directCommand = utils::InFile::readAll(pathToCommand) + "brick.run()";
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
}

void Display::setBackground(QColor const &color)
{
	QString const pathToCommand = ":/trik/templates/drawing/setBackground.t";
	qDebug() << pathToCommand << "\n";
	QString directCommand = utils::InFile::readAll(pathToCommand).replace("@@COLOR@@", color.name()) + "brick.run()";
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
}

void Display::clearScreen()
{
	///Some questions here (Me)
	QString const pathToCommand = ":/trik/templates/drawing/clearUsedScreen.t";
	qDebug() << pathToCommand << "\n";
	QString directCommand = utils::InFile::readAll(pathToCommand) + "brick.run()";
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
}

void Display::printText(int x, int y, QString const &text)
{
	QString const pathToCommand = ":/trik/templates/drawing/printText.t";
	qDebug() << pathToCommand << "\n";
	QString directCommand = utils::InFile::readAll(pathToCommand);
	directCommand = directCommand.replace("@@TEXT@@", text).replace("@@X@@", QString::number(x))
						.replace("@@Y@@", QString::number(y)) + "brick.run()";
	qDebug() << directCommand << "\n";
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
}
