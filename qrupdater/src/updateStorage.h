#pragma once

#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtCore/QObject>
#include <QtCore/QDir>

#include "update.h"

namespace qrUpdater {
//!
//! @brief The UpdateStorage class
//! Saving updates for later usage and loading this information back
//! using QSettings as IniFormat
class UpdateStorage : public QObject
{
	Q_OBJECT
public:
	UpdateStorage(QString updatesFolder, QObject *parent);
	~UpdateStorage();

	//! saves all info of current unit in parser, moves setup-file to storage folder
	void saveFileForLater(Update *concreteUpdate, QString const filePath);
	//! after-install cleaning
	void removeUpdate(Update *update);

	//! @return True is exists file with update descriptions
	bool hasPreparedUpdatesInfo();
	//! loads mPreparedUpdates
	void loadUpdatesInfo(QStringList const units);

	QList<Update *> preparedUpdates() const;

protected:
	void saveUpdateInfo(Update *update);

	QList<Update *> mPreparedUpdates;
	QString const mUpdatesFolder;
	QString const settingsFile;
	QSettings *mUpdateInfo;
};

}
