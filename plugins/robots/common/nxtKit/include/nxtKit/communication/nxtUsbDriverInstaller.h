/* Copyright 2016 CyberTech Labs Ltd.
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

#include <QtCore/QProcess>

namespace nxt {
namespace communication {

/// Class that handles USB driver installation for USB brick.
class NxtUsbDriverInstaller : public QObject
{
	Q_OBJECT

public:
	/// Attempts to initiate NXT USB drivers installation process. All errors and user communication is implemented
	/// inside. When installation process is complete (successfully or not) installationFinished()
	/// signal will be emitted.
	/// @note This object must be in GUI thread cause it may produce message boxes prompted to user.
	bool installUsbDriver();

signals:
	/// Emitted when drivers installion process has finished, successfully or not.
	void installationFinished(bool success);

	/// Emitted when some errors occured during drivers installation process.
	void errorOccured(const QString &message);

	/// Emitted when some informational message must be shown to user.
	void messageArrived(const QString &message);

private:
	QString path(const QString &file = QString()) const;

	bool promptDriverInstallation() const;
	QString checkWindowsDriverComponents() const;

	QString findBossaProgramPortDriver() const;

	QProcess mInstallationProcess;
};

}
}
