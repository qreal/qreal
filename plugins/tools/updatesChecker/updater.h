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
