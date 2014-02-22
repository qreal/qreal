#include "updateStorage.h"

namespace keys {
static QString const filePath = "fileName";
static QString const version = "version";
static QString const arguments = "args";
}

using namespace qrUpdater;

UpdateStorage::UpdateStorage(QString updatesFolder, QObject *parent)
	: QObject(parent)
	, mUpdatesFolder(updatesFolder)
	, settingsFile(updatesFolder + "updateInfo.ini")
{
	mUpdateInfo = new QSettings(settingsFile, QSettings::IniFormat, parent);
}

void UpdateStorage::saveUpdateInfo(Update *update)
{
	mUpdateInfo->beginGroup(update->unit());
	mUpdateInfo->setValue(keys::filePath, update->filePath());
	mUpdateInfo->setValue(keys::version, update->version());
	mUpdateInfo->setValue(keys::arguments, update->arguments());
	mUpdateInfo->endGroup();
}

void UpdateStorage::saveFileForLater(Update *concreteUpdate, QString const filePath)
{
	QDir().mkdir(mUpdatesFolder);

	if (QFile::exists(mUpdatesFolder + QFileInfo(filePath).fileName())) {
		QFile::remove(mUpdatesFolder + QFileInfo(filePath).fileName());
	}

	QString const endFilePath = mUpdatesFolder + QFileInfo(filePath).fileName();

	QFile::rename(filePath, endFilePath);

	concreteUpdate->setFilePath(endFilePath);
	mPreparedUpdates << concreteUpdate;
	saveUpdateInfo(concreteUpdate);
}

void UpdateStorage::removeUpdate(Update *update)
{
	mUpdateInfo->remove(update->unit());
	update->clear();
	mPreparedUpdates.removeOne(update);
}

bool UpdateStorage::hasPreparedUpdatesInfo()
{
	return QDir(mUpdatesFolder).exists() && QFile::exists(settingsFile);
}

void UpdateStorage::loadUpdatesInfo(QStringList const units)
{
	if (!hasPreparedUpdatesInfo()) {
		return;
	}

	foreach (QString const unit, units) {
		Update *newUpdate = new Update(this);

		mUpdateInfo->beginGroup(unit);
		newUpdate->setData(
				mUpdateInfo->value(keys::filePath).toString()
				, mUpdateInfo->value(keys::arguments).toStringList()
				, mUpdateInfo->value(keys::version).toString()
		);
		newUpdate->setUnitName(unit);
		mUpdateInfo->endGroup();

		if (!newUpdate->isEmpty()) {
			mPreparedUpdates << newUpdate;
		} else {
			delete newUpdate;
		}
	}
}

void UpdateStorage::sync()
{
	foreach (Update *update, mPreparedUpdates) {
		if (update->isEmpty()) {
			mUpdateInfo->remove(update->unit());
		}
	}
	mUpdateInfo->sync();
	if (QDir(mUpdatesFolder).exists() && QFile::exists(settingsFile) && QFile(settingsFile).size() == 0) {
		QFile::remove(settingsFile);
		QDir(mUpdatesFolder).removeRecursively();
	}
}

QList<Update *> UpdateStorage::preparedUpdates() const
{
	return mPreparedUpdates;
}

