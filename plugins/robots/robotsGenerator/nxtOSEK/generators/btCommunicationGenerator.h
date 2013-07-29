#pragma once

#include <QtCore/QString>

class BtCommunicationGenerator
{
public:
	BtCommunicationGenerator();
	void btMasterUsed();
	void btSlaveUsed();
	void reinit();
	void generateBtCommCode(QString &resultOil, QString &resultString, QString &taskTemplate);

private:
	bool mIsBtMaster;
	bool mIsBtSlave;
};
