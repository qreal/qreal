#pragma once

#include <QtCore/QIODevice>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>

#include "update.h"

//!
//! @brief The DetailsParser class
//! reads received file and provides fast access to main params
class DetailsParser : public QObject
{
	Q_OBJECT
public:
	DetailsParser();
	virtual ~DetailsParser(){}

	//! switch to other result of parsing
	//! @param unit other module name
	virtual void changeUnit(QString const unit);

	//! @return true in case input is invalid
	virtual bool hasErrors() const = 0;

	//! @return Update - result of parsing
	Update* currentUpdate() const;

	//! @return all units saved in options-file
	QStringList units() const;
	QString currentUnit() const;

signals:
	void parseFinished();

public slots:
	//! starts parsing
	virtual void parseDevice(QIODevice *device) = 0;

protected:
	Update *mCurrentUpdate;
	//! unit <-> url
	QMap<QString, QUrl> mFileUrls;
	QMap<QString, QString> mParamStrings;
	QMap<QString, QString> mVersions;
};

