#include "display.h"
#include <qrutils/inFile.h>
#include <QtGui/QColor>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Display::Display(DeviceInfo const &info, PortInfo const &port
	, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikDisplay(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void Display::drawSmile(bool sad)
{
	QString const pathToCommand = sad ? ":/trik/templates/drawing/sadSmile.t" : ":/trik/templates/drawing/smile.t";
	QString const directCommand = utils::InFile::readAll(pathToCommand);
	mRobotCommunicator.runDirectCommand(directCommand);
}

void Display::setBackground(QColor const &color)
{
	QString const pathToCommand = ":/trik/templates/drawing/setBackground.t";
	QString const directCommand = utils::InFile::readAll(pathToCommand)
		.replace("@@COLOR@@", color.name());
	mRobotCommunicator.runDirectCommand(directCommand);
}

void Display::clearScreen()
{
	QString const pathToCommand = ":/trik/templates/drawing/clearUsedScreen.t";
	QString const directCommand = utils::InFile::readAll(pathToCommand);
	mRobotCommunicator.runDirectCommand(directCommand);
}

void Display::printText(int x, int y, QString const &text)
{
	QString const pathToCommand = ":/trik/templates/drawing/printText.t";
	QString const directCommand = utils::InFile::readAll(pathToCommand).replace("@@TEXT@@", text)
		.replace("@@X@@", QString::number(x)).replace("@@Y@@", QString::number(y)) + "brick.run();";
	mRobotCommunicator.runDirectCommand(directCommand);
}
