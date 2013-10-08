#pragma once

#include <QtCore/QUrl>
#include <QtCore/QProcess>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

//!
//! @brief The Update class
//! Stores one-unit update informatiom and provides installing
class Update : public QObject
{
	Q_OBJECT
public:
	explicit Update(QObject *parent = 0);
	Update(QString const filePath, QStringList const args, QString const version, QObject *parent = 0);

	void setUrl(QUrl const link);
	void setUnitName(QString const unit);
	void setData(QString const filePath, QStringList const args, QString const version, QUrl const link = QUrl());
	//! removes update-file and clears all information
	void clear();

	//! starts setup process, emits signal when finished
	void installUpdate();

	//! @return True case there is no file for install
	bool isEmpty() const;
	bool isInstalling() const;
	bool isInstalled() const;
	QUrl url() const;
	QString unit() const;
	QString filePath() const;
	QString fileName() const;
	QString version() const;
	QStringList arguments() const;

signals:
	//! @return True if process finished correctly
	void installFinished(bool);

protected:
	bool mIsInstalled;
	QString mFilePath;
	QStringList mArguments;
	QString mVersion;
	QString mModule;
	QUrl mDownloadUrl;
	QProcess *mProcess;

protected slots:
	void installingFinished(int exitCode, QProcess::ExitStatus status);
};

