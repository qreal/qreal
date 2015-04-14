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
