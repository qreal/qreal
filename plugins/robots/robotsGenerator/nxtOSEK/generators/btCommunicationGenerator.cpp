#include "btCommunicationGenerator.h"

BtCommunicationGenerator::BtCommunicationGenerator()
{
	reinit();
}


void BtCommunicationGenerator::btMasterUsed()
{
	mIsBtMaster = true;
}

void BtCommunicationGenerator::btSlaveUsed()
{
	mIsBtSlave = true;
}

void BtCommunicationGenerator::reinit()
{
	mIsBtMaster = false;
	mIsBtSlave = false;
}

void BtCommunicationGenerator::generateBtCommCode(QString &resultOil, QString &resultString, QString &taskTemplate)
{
	if (mIsBtMaster || mIsBtSlave) {
		resultOil.replace("PRIORITY = 1; /* lowest priority */", "PRIORITY = 2;\n\tEVENT = BtConnect;");
		resultOil.replace("@@TASK@@", taskTemplate.replace("OSEK_Task_Number_0", "IdleTask")
		+ "\n" + "@@TASK@@");
		resultOil.replace("@@EVENT@@", "EVENT BtConnect\n{\n\tMASK = AUTO;\n};");

		resultString.replace("@@TASKDECLARATION@@", "DeclareTask(IdleTask);\n@@TASKDECLARATION@@");
		resultString.replace("@@TASKDECLARATION@@", "DeclareEvent(BtConnect);\n@@TASKDECLARATION@@");
		resultString.replace("@@DEFINES@@", "#define RUNTIME_CONNECTION\n@@DEFINES@@");
		resultString.replace("@@TERMINATEHOOKS@@", "\tecrobot_term_bt_connection();");
		resultString += "\nTASK(IdleTask)\n{\n\tstatic SINT bt_status = BT_NO_INIT;\n\twhile(1)\n\t{\n#ifdef RUNTIME_CONNECTION\n";
		if(mIsBtMaster) {
			resultString.replace("@@INITHOOKS@@", "#ifndef RUNTIME_CONNECTION\n\tecrobot_init_bt_master(bd_addr, \"LEJOS-OSEK\");\n#endif");
			resultString += "\t\tecrobot_init_bt_master(bd_addr, \"LEJOS-OSEK\");\n#endif\n";
		} else {
			resultString.replace("@@INITHOOKS@@", "#ifndef RUNTIME_CONNECTION\n\tecrobot_init_bt_slave(\"LEJOS-OSEK\");\n#endif");
			resultString += "\t\tecrobot_init_bt_slave(\"LEJOS-OSEK\");\n#endif\n";
		}
		resultString += "\tif (ecrobot_get_bt_status() == BT_STREAM && bt_status != BT_STREAM)\n";
		resultString += "\t{\n\t\tSetEvent(OSEK_Task_Number_0, BtConnect);\n\t}";
		resultString += "\n\tbt_status = ecrobot_get_bt_status();\n\t}\n}";
	}
}
