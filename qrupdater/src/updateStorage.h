#pragma once

#include <QtCore/QSettings>
#include <QtCore/QObject>
#include <QtCore/QDir>

#include "detailsParser.h"

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
	void saveFileForLater(DetailsParser const *parser, QString const filePath);
	//! after-install cleaning
	void removePreparedUpdate();

	//! @return True is exists file with update descriptions
	bool hasPreparedUpdatesInfo();
	//! loads mPreparedUpdate var
	void loadUpdateInfo(QString const unit);

	Update* preparedUpdate();

protected:
	void saveInfoFromParser(DetailsParser const *parser);

	Update *mPreparedUpdate;
	QString const mUpdatesFolder;
	QString const settingsFile;
	QSettings *mUpdateInfo;
};

