#include "buildOnVirtualMachineBlock.h"

#include <QtCore/QCoreApplication>

using namespace deployment::blocks;

BuildOnVirtualMachineBlock::BuildOnVirtualMachineBlock(ShellWidget *shellWidget)
	: ShellBlock(shellWidget)
{
}

QStringList BuildOnVirtualMachineBlock::arguments()
{
	QStringList result = {
		QCoreApplication::applicationDirPath() + "/deployment-scripts/virtualbox/build.sh"
		, stringProperty(id(), "OsType")
		, stringProperty(id(), "ProductName")
		, eval<QString>("Version")
	};

	QString const vdi = eval<QString>("VDI");
	QString const sharedFolder = eval<QString>("SharedFolder");
	if (!vdi.isEmpty() && !sharedFolder.isEmpty()) {
		result << vdi << sharedFolder;
	}

	return result;
}
