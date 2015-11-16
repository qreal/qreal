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

#include "updater.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtXml/QDomDocument>

#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>

using namespace updatesChecker;

Updater::Updater(QObject *parent)
	: QObject(parent)
	, mUpdaterProcess(nullptr)
{
}

void Updater::checkForNewVersion()
{
	executeUpdater("--checkupdates");
}

void Updater::start()
{
	executeUpdater("--updater");
}

void Updater::executeUpdater(const QString &mode)
{
	mUpdaterProcess = new QProcess(this);
	mUpdaterProcess->setWorkingDirectory(qReal::PlatformInfo::applicationDirPath());
	connect(mUpdaterProcess, SIGNAL(finished(int)), this, SLOT(readAnswer()));

	if (mode == "--updater") {
		// Executing updater in detached mode (to close application just after).
		mUpdaterProcess->startDetached(qReal::PlatformInfo::invariantSettingsPath("pathToUpdater"), {mode});
	} else {
		mUpdaterProcess->start(qReal::PlatformInfo::invariantSettingsPath("pathToUpdater"), {mode});
	}
}

void Updater::readAnswer()
{
	const QString output = mUpdaterProcess->readAllStandardOutput();
	// Checking that output is a valid XML
	QDomDocument parser;
	parser.setContent(output);
	QLOG_INFO() << "Updater output:" << output;
	if (!output.isEmpty() && !parser.isNull() && output.trimmed().startsWith("<")) {
		emit newVersionAvailable();
	} else {
		emit noNewVersionAvailable();
	}
}
