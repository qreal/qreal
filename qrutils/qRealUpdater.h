#pragma once

#include <QtCore/QProcess>

#include <qrkernel/version.h>

#include "utilsDeclSpec.h"

namespace utils {

/// Starts and interacts with qrupdater component.
class QRUTILS_EXPORT QRealUpdater : public QObject
{
	Q_OBJECT

public:
	/// @param applicationPath The path to QReal executable file.
	/// @param unit The name of concrete CASE solution (for example qRealRobots.RobotsPlugin).
	/// @param version The version of the concrete CASE solution. Versions on the server will be compared to this one.
	QRealUpdater(QString const &applicationPath
			, QString const &unit
			, qReal::Version const &version
			, QObject *parent = 0);

	/// Updater will just perform light-weight versions comparison network operation.
	/// If new version of the unit given in the constructor is available newVersionAvailable()
	/// signal will be emitted.
	/// @param downnloadIfFound If this argument is true then new version installers will be
	/// downloaded in the background if they exist.
	void checkForNewVersion(bool downloadIfFound);

	/// Installs already downloaded updates. The environment must be
	/// shutted down before this operation will be started.
	void installUpdates();

	/// Downloads and installs new updates if sych exist. The environment must be
	/// shutted down before this operation will be started.
	void downloadAndInstall();

signals:
	void newVersionAvailable(qReal::Version const &version);

private slots:
	void readAnswer();

private:
	void executeUpdater(QString const &mode);
	QStringList arguments(QString const &mode);

	QString const mQRealPath;
	QString const mUpdaterPath;
	QString const mUnit;
	bool mMustBeDownloadedAfterCheck;
	qReal::Version const mVersion;
	QProcess *mUpdaterProcess;  // Takes ownership
};

}
