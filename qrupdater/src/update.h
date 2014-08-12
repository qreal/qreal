#pragma once

#include <QtCore/QUrl>
#include <QtCore/QProcess>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

#include <qrkernel/version.h>

namespace qrUpdater {

/// Stores one-unit update information and performs its installation.
class Update : public QObject
{
	Q_OBJECT
public:
	explicit Update(QObject *parent = 0);
	Update(QString const &filePath, QStringList const &args, QString const &version, QObject *parent = 0);

	void setUrl(QUrl const &link);
	void setUnitName(QString const &unit);
	void setFilePath(QString const &path);
	void setData(QString const &filePath, QStringList const &args, QString const &version, QUrl const &link = QUrl());
	/// Removes update-file and clears all information.
	void clear();

	/// Starts setup process, emits signal when finished.
	void installUpdate();

	/// @return True case there is no file for install.
	bool isEmpty() const;
	bool isInstalling() const;
	bool isInstalled() const;
	bool hasSelfInstallMarker() const;
	QUrl url() const;
	QString unit() const;
	QString filePath() const;
	QString fileName() const;
	qReal::Version version() const;
	QStringList &arguments();

signals:
	/// @arg True if process finished correctly.
	void installed(bool);

protected:
	bool mIsInstalled;
	QString mFilePath;
	QStringList mArguments;
	qReal::Version mVersion;
	QString mModule;
	QUrl mDownloadUrl;
	QProcess *mProcess;

protected slots:
	void installationFinished(int exitCode, QProcess::ExitStatus status);
};

}
