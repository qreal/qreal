/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
