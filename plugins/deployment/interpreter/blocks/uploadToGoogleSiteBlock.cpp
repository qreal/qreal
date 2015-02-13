#include "uploadToGoogleSiteBlock.h"

#include <QtCore/QCoreApplication>

using namespace deployment::blocks;

UploadToGoogleSiteBlock::UploadToGoogleSiteBlock(ShellWidget *shellWidget)
	: ShellBlock(shellWidget)
{
}

QString UploadToGoogleSiteBlock::processName() const
{
	return "python2";
}

QStringList UploadToGoogleSiteBlock::arguments()
{
	QStringList const urls = eval<QStringList>("Urls");
	QStringList const titles = eval<QStringList>("Titles");
	QStringList const descriptions = eval<QStringList>("Descriptions");
	if (urls.count() != titles.count() || urls.count() != descriptions.count()) {
		error(tr("URLs, titles and descriptions elements count must be equal each other"));
		return {};
	}

	if (urls.count() == 0) {
		error(tr("URLs, titles and descriptions collections must contain at least one item"));
		return {};
	}

	QStringList result = {
		QCoreApplication::applicationDirPath() + "/deployment-scripts/uploading/upload_to_google_site.py"
		, stringProperty(id(), "Site")
		, eval<QString>("Username")
		, eval<QString>("Password")
	};

	for (int i = 0; i < urls.count(); ++i) {
		result << urls[i];
		result << titles[i];
		result << descriptions[i];
	}

	return result;
}
