#include "updateDownloadsCounterBlock.h"

#include <QtCore/QCoreApplication>

using namespace deployment::blocks;

UpdateDownloadsCounterBlock::UpdateDownloadsCounterBlock(ShellWidget *shellWidget)
	: ShellBlock(shellWidget)
{
}

QStringList UpdateDownloadsCounterBlock::arguments()
{
	QStringList const keys = eval<QStringList>("Keys");
	QStringList const urls = eval<QStringList>("Urls");
	if (keys.count() != urls.count()) {
		error(tr("Keys count must be equal URLs count"));
		return {};
	}

	if (keys.count() == 0) {
		error(tr("Keys and URLs collections must contain at least one item"));
		return {};
	}

	QString const windowsOfflineKey = keys[0].toLower();
	if (!windowsOfflineKey.contains("windows") || !windowsOfflineKey.contains("offline")) {
		error(tr("Firts key-URL pair must specify windows offline installer"));
		return {};
	}

	QStringList result = {
		QCoreApplication::applicationDirPath() + "/deployment-scripts/uploading/upload_to_google_site.sh"
		, stringProperty(id(), "Mode")
		, stringProperty(id(), "NewVersion")
		, stringProperty(id(), "SshKey")
	};

	for (int i = 0; i < keys.count(); ++i) {
		result << keys[i];
		result << urls[i];
	}

	return result;
}
