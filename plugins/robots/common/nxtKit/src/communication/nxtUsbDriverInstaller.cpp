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

#include "nxtKit/communication/nxtUsbDriverInstaller.h"

#include <QtCore/QDirIterator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>
#include <qrkernel/settingsManager.h>

using namespace nxt::communication;

const QString bossaDriverDescription = "Arduino Srl \\(www\\.arduino\\.org\\)";

QString NxtUsbDriverInstaller::path(const QString &file) const
{
	return QDir::toNativeSeparators(qReal::PlatformInfo::invariantSettingsPath("pathToNxtTools") + "/" + file);
}

bool NxtUsbDriverInstaller::installUsbDriver()
{
#ifdef Q_OS_WIN
	QLOG_WARN() << "Driver for LEGO device not found. Trying to installing WinUSB...";

	if (mInstallationProcess.state() != QProcess::NotRunning) {
		QLOG_ERROR() << "Attempted to install NXT driver during installation already running, that's strange";
		return false;
	}

	const QString validationError = checkWindowsDriverComponents();
	if (!validationError.isEmpty()) {
		QLOG_ERROR() << validationError << "Probably NXT tools are not installed.";
		emit errorOccured(tr("Driver for NXT is not installed. An attempt to attach TRIK Studio "\
				"driver also failed (probably NXT tools package was not installer). No panic! Driver can still be "\
				"installed manually, see documentation, chapter \"Installing NXT driver manually.\". Also TRIK Studio "\
				"supports <a href='%1'>Lego Fantom driver</a>, you can just download and install it."));
		emit installationFinished(false);
		return false;
	}

	if (!promptDriverInstallation()) {
		emit messageArrived(tr("Driver installation cancelled. Please note that TRIK Studio also supports "\
				"official <a href='%1'>Lego Fantom driver</a>, you can just download and install it.")
				.arg(qReal::SettingsManager::value("fantomDownloadLink").toString()));
		QLOG_WARN() << "User cancelled driver installation.";
		emit installationFinished(false);
		return false;
	}

	disconnect(&mInstallationProcess);

	const QString bossaDriverName = findBossaProgramPortDriver();
	const QString pnpUtilPath = qReal::PlatformInfo::isX64() ? "%windir%\\sysnative\\PnPutil.exe" : "PnPutil.exe";
	const QString deleteBossaDriverCmd = bossaDriverName.isEmpty()
			? QString()
			: QString("%1 -f -d %2 & ").arg(pnpUtilPath, bossaDriverName);

	// Installation process is just running .inf files with administrator permissions.
	// elevate.exe calls UAC screen and executes pnputil to install drivers from 'driver' folder of nxt-tools.
	const QString installDriversCmd = QString("%1 -w -c \"\"cmd /q /c \""\
			"\"%2%3 -i -a %4\\*.inf\"\" \"\"")
			.arg(path("driver\\elevate.exe"), deleteBossaDriverCmd, pnpUtilPath, path(QString("driver")));
	QLOG_INFO() << "Elevating pnputil... Using command" << installDriversCmd;
	connect(&mInstallationProcess, QProcess::readyRead, this, [=]() {
		QLOG_INFO() << "NXT drivers installer:" << mInstallationProcess.readAll();
	});
	connect(&mInstallationProcess, static_cast<void(QProcess::*)(int)>(&QProcess::finished), this, [=](int exitCode) {
		QLOG_INFO() << "NXT drivers installation finished with exit code" << exitCode;
		if (exitCode != 0) {
			emit errorOccured(tr("An attempt to attach TRIK Studio driver failed. No panic! Driver can be still "\
					"installed manually, see documentation, chapter \"Installing NXT driver manually.\". Also "\
					"TRIK Studio supports <a href='%1'>Lego Fantom driver</a>, you can just download and install it.")
					.arg(qReal::SettingsManager::value("fantomDownloadLink").toString()));
		}

		emit installationFinished(exitCode == 0);
	});

	mInstallationProcess.setEnvironment(QProcess::systemEnvironment());
	mInstallationProcess.start(installDriversCmd);
	if (!mInstallationProcess.waitForStarted()) {
		QLOG_ERROR() << "Could not waitForStarted() elevate.exe.";
		emit installationFinished(false);
		return false;
	}

	return true;
#else
	return true;
#endif
}

bool NxtUsbDriverInstaller::promptDriverInstallation() const
{
	const QString title = tr("NXT drivers not found");
	const QString question = tr("Drivers for LEGO NXT brick are not installed. TRIK Studio can install own drivers to "\
			"communicate with NXT. Do you want to do it?");
	const QString warning = tr("TRIK Studio drivers are not officially registered, so two red warning messages will "\
			"be shown by Windows. Confirm them to proceed installation.");
	if (QMessageBox::question(QApplication::focusWidget(), title, question) == QMessageBox::Yes) {
		QMessageBox::information(QApplication::focusWidget(), title, warning);
		return true;
	}

	return false;
}

QString NxtUsbDriverInstaller::checkWindowsDriverComponents() const
{
	const QStringList components = {
		path("driver\\lego_firmware_winusb_install.inf")
		, path("driver\\lego_firmware_winusb_install.cat")
		, path("driver\\lego_winusb_install.inf")
		, path("driver\\lego_winusb_install.cat")
		, path("driver\\elevate.exe")
	};

	for (const QString &component : components) {
		if (!QFile(component).exists()) {
			return QString("%1 does not exist, cannot install drivers.").arg(component);
		}
	}

	return QString();
}

QString NxtUsbDriverInstaller::findBossaProgramPortDriver() const
{
#ifdef Q_OS_WIN
	// In lastest Windows versions Arduino driver attached to resetted NXT by default. This is the reason
	// why all Lego NXT users have nightmare with resetting NXT firmware. Here we search for the name of Arduino
	// driver attached to NXT device.
	QLOG_INFO() << "Searching for Bossa Program Port driver attached...";
	QStringList data;
	QString result;
	QProcess pnpUtil;
	connect(&pnpUtil, &QProcess::readyRead, this, [&pnpUtil, &data]() { data << pnpUtil.readAll(); });
	// Simply starting pnputil -e will return answer in default locale, so changin locale to english with 'chcp 437'.
	const QString pnpUtilPath = qReal::PlatformInfo::isX64() ? "%windir%\\sysnative\\PnPutil.exe" : "PnPutil.exe";
	pnpUtil.start("cmd", { "/c", QString("chcp 437 & %1 -e").arg(pnpUtilPath) }, QProcess::ReadOnly);
	if (!pnpUtil.waitForStarted()) {
		QLOG_ERROR() << "Could not spawn pnputil process. Args:" << pnpUtil.arguments();
		return QString();
	}

	if (!pnpUtil.waitForFinished()) {
		QLOG_ERROR() << "pnpUtil.exe timed out (strangely). Args:" << pnpUtil.arguments();
		return QString();
	}

	QRegExp driverNameRegexp(QString(".*Published name :\\s+([A-Za-z0-9_\\.]+)\\s+Driver package provider :\\s+%1.*")
			.arg(bossaDriverDescription));
	if (driverNameRegexp.exactMatch(data.join('\n'))) {
		QLOG_INFO() << "Found" << bossaDriverDescription << "entry:" << result;
		result = driverNameRegexp.cap(1);
	} else {
		QLOG_INFO() << bossaDriverDescription << "entry not found";
	}

	return result;
#else
	return QString();
#endif
}
