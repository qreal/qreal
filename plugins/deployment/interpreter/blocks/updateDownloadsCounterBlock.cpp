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
		QCoreApplication::applicationDirPath() + "/deployment-scripts/uploading/update_site_scripts.sh"
		, stringProperty(id(), "Mode")
		, eval<QString>("NewVersion")
		, eval<QString>("SshKey")
	};

	for (int i = 0; i < keys.count(); ++i) {
		result << keys[i];
		result << urls[i];
	}

	return result;
}
