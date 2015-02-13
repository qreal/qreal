#include "uploadToGoogleDriveBlock.h"

#include <QtCore/QCoreApplication>

using namespace deployment::blocks;

UploadToGoogleDriveBlock::UploadToGoogleDriveBlock(ShellWidget *shellWidget)
	: ShellBlock(shellWidget)
	, mUrlRegexp("^Uploaded successfully, URL to download: (.*)$")
{
	connect(mProcess, &QProcess::readyReadStandardOutput, [=]() {
		if (mUrlRegexp.exactMatch(mLastOutput)) {
			evalCode(stringProperty(id(), "ResultingVariable") + " = \"" + mUrlRegexp.cap(1).trimmed() + "\"");
		}
	});
}

QStringList UploadToGoogleDriveBlock::arguments()
{
	return {
		QCoreApplication::applicationDirPath() + "/deployment-scripts/uploading/upload_to_google_drive.sh"
		, eval<QString>("File")
		, eval<QString>("FolderId")
		, eval<QString>("Username")
		, eval<QString>("Password")
	};
}
