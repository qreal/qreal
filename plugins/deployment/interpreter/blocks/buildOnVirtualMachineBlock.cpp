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
		, stringProperty(id(), "Version")
	};
	QString const vdi = stringProperty(id(), "VDI");
	QString const sharedFolder = stringProperty(id(), "SharedFloder");
	if (!vdi.isEmpty() && !sharedFolder.isEmpty()) {
		result << vdi << sharedFolder;
	}

	return result;
}
