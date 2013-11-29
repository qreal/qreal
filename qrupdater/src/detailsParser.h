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

	//! @param unit module name
	//! @returns Update from list or NULL if does not exist
	Update* update(QString const unit);
	Update* update(QUrl const url);

	//! @return true in case input is invalid
	virtual bool hasErrors() const = 0;

	//! @return all units saved in options-file
	QStringList units() const;
	QList<Update *> updatesParsed() const;

signals:
	void parseFinished();

public slots:
	//! starts parsing
	virtual void processDevice(QIODevice *device);

protected:
	virtual void parseDevice(QIODevice *device) = 0;

	QList<Update *> mUpdates;

	//! unit <-> url
	QMap<QString, QUrl> mFileUrls;
	QMap<QString, QString> mParamStrings;
	QMap<QString, QString> mVersions;
};

