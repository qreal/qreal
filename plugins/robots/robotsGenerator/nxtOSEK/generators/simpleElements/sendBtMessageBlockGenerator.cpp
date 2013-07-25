#include "sendBtMessageBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

SendBtMessageBlockGenerator::SendBtMessageBlockGenerator()
{
}

QList<SmartLine> SendBtMessageBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	Q_UNUSED(logicElementId)
	QList<SmartLine> result;
	QString const message = nxtGen->api()->stringProperty(logicElementId, "Message").toUtf8();
	nxtGen->variables().hasBtSend();
	int len = message.length();
	if(len > 0 && len <= 32) {
		QString lenStr = QString::number(len);
		QString arrStr = "";
		for(int i = 0; i < len; i++) {
			QString hexadecimal;
			hexadecimal.setNum(message.at(i).unicode(), 16);
			arrStr += "0x" + hexadecimal;
			if (i < len - 1) {
				arrStr += ", ";
			}
		}

		result.append(SmartLine("const U8 message[" + lenStr + "] = {" + arrStr + "};" + " //message \"" + message + "\" in ascii", elementId));
		result.append(SmartLine("memcpy(bt_send_buf, message, " + lenStr + ");", elementId));
	}

	if (len > 32) {
		result.append(SmartLine("\\message is too long (length is more then 32)", elementId));
	}

	result.append(SmartLine("ecrobot_send_bt_packet(bt_send_buf, 32);", elementId));

	return result;
}
