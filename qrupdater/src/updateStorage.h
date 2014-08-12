#pragma once

#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtCore/QObject>
#include <QtCore/QDir>

#include "update.h"

namespace qrUpdater {

/// Saves updates for later usage and loading this information back using QSettings in .ini format
class UpdateStorage : public QObject
{
	Q_OBJECT

public:
	UpdateStorage(QString const &updatesFolder, QObject *parent);

	/// Saves all info of current unit in parser, moves setup-file to storage folder.
	void saveFileForLater(Update *concreteUpdate, QString const &filePath);

	/// Performs after-install cleaning.
	void removeUpdate(Update *update);

	/// @return True if file with update descriptions exists
	bool hasPreparedUpdatesInfo();

	/// Loads mPreparedUpdates
	void loadUpdatesInfo(QStringList const &units);

	void sync();

	QList<Update *> preparedUpdates() const;

protected:
	void saveUpdateInfo(Update *update);

	QList<Update *> mPreparedUpdates;
	QString const mUpdatesFolder;
	QString const mSettingsFile;
	QSettings *mUpdateInfo;
};

}
