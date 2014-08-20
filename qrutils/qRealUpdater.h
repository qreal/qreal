#pragma once

#include <qrkernel/version.h>

#include "utilsDeclSpec.h"

class QProcess;

namespace utils {

/// Starts and interacts with the maintenance component.
class QRUTILS_EXPORT QRealUpdater : public QObject
{
	Q_OBJECT

public:
	/// @param applicationPath The path to QReal executable file.
	/// @param unit The name of concrete CASE solution (for example qRealRobots.RobotsPlugin).
	/// @param version The version of the concrete CASE solution. Versions on the server will be compared to this one.
	explicit QRealUpdater(QObject *parent = 0);

	/// Updater will just perform light-weight versions comparison network operation.
	/// If new updates are available newVersionAvailable() signal will be emitted.
	void checkForNewVersion();

	/// Installs already downloaded updates. The environment must be
	/// shutted down before this operation will be started.
	void start();

signals:
	void newVersionAvailable();

private slots:
	void readAnswer();

private:
	void executeUpdater(QString const &mode);

	QProcess *mUpdaterProcess;  // Takes ownership
};

}
