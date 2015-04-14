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

#pragma once

#include <qrkernel/version.h>

class QProcess;

namespace updatesChecker {

/// Starts and interacts with the maintenance component.
class Updater : public QObject
{
	Q_OBJECT

public:
	explicit Updater(QObject *parent = 0);

	/// Updater will just perform light-weight versions comparison network operation.
	/// If new updates are available newVersionAvailable() signal will be emitted.
	void checkForNewVersion();

	/// Installs already downloaded updates. The environment must be
	/// shutted down before this operation will be started.
	void start();

signals:
	/// Emitted when new version was found on remote server
	void newVersionAvailable();
	/// Emitted when no new version was found on remote server
	void noNewVersionAvailable();

private slots:
	void readAnswer();

private:
	void executeUpdater(const QString &mode);

	QProcess *mUpdaterProcess;  // Takes ownership
};

}
