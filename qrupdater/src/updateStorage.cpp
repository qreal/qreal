#include "updateStorage.h"

UpdateStorage::UpdateStorage(QString updatesFolder, QObject *parent)
	: QObject(parent)
	, mUpdatesFolder(updatesFolder)
	, settingsFile(updatesFolder + "updateInfo.ini")
{
	mPreparedUpdate = new Update(this);
	mUpdateInfo = new QSettings(settingsFile, QSettings::IniFormat, parent);
}

UpdateStorage::~UpdateStorage()
{
	mUpdateInfo->sync();
	if (QDir(mUpdatesFolder).exists() && QFile::exists(settingsFile) && QFile(settingsFile).size() == 0) {
		QFile::remove(settingsFile);
		QDir(mUpdatesFolder).removeRecursively();
	}
}

void UpdateStorage::saveInfoFromParser(DetailsParser const *parser)
{
	mUpdateInfo->beginGroup(parser->currentUnit());
	mUpdateInfo->setValue("fileName", parser->currentUpdate()->filePath());
	mUpdateInfo->setValue("version", parser->currentUpdate()->version());
	mUpdateInfo->setValue("args", parser->currentUpdate()->arguments());
	mUpdateInfo->endGroup();
}

void UpdateStorage::saveFileForLater(DetailsParser const *parser, QString const filePath)
{
	QDir().mkdir(mUpdatesFolder);

	if (QFile::exists(mUpdatesFolder + QFileInfo(filePath).fileName())) {
		QFile::remove(mUpdatesFolder + QFileInfo(filePath).fileName());
	}

	QFile::rename(filePath, mUpdatesFolder + QFileInfo(filePath).fileName());

	saveInfoFromParser(parser);
}

void UpdateStorage::removePreparedUpdate()
{
	mUpdateInfo->remove(mPreparedUpdate->unit());
	mPreparedUpdate->clear();
}

bool UpdateStorage::hasPreparedUpdatesInfo()
{
	return QDir(mUpdatesFolder).exists() && QFile::exists(settingsFile);
}

void UpdateStorage::loadUpdateInfo(QString const unit)
{
	if (!hasPreparedUpdatesInfo()) {
		return;
	}

	mUpdateInfo->beginGroup(unit);
	mPreparedUpdate->setData(
			mUpdatesFolder + mUpdateInfo->value("fileName").toString()
			, mUpdateInfo->value("args").toStringList()
			, mUpdateInfo->value("version").toString()
	);
	mPreparedUpdate->setUnitName(unit);
	mUpdateInfo->endGroup();
}

Update *UpdateStorage::preparedUpdate()
{
	return mPreparedUpdate;
}

