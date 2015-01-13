#include "uploadToGoogleDriveBlock.h"

#include <QtCore/QCoreApplication>

#include <qrutils/stringUtils.h>

using namespace deployment::blocks;

UploadToGoogleDriveBlock::UploadToGoogleDriveBlock(ShellWidget *shellWidget)
	: ShellBlock(shellWidget)
{
}

QStringList UploadToGoogleDriveBlock::arguments()
{
	return {
		QCoreApplication::applicationDirPath() + "/deployment-scripts/uploading/upload_to_google_drive.sh"
		, eval<QString>("File")
		, stringProperty(id(), "FolderId")
		, stringProperty(id(), "Username")
		, stringProperty(id(), "Password")
	};
}
