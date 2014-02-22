#pragma once

#include <QtCore/QStringList>
#include <QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>

#include "../qrkernel/settingsManager.h"

class QRealUpdater : public QObject
{
	Q_OBJECT
public:
	explicit QRealUpdater(char *applicationPath);

	bool hasUpdates();

	void startUpdater();

protected:
	bool hasUpdatePermission();
	void executeUpdater();
	void transferInfo();

	void prepareForClose();

	static int const updaterTimeout = 1000;
	bool mHasNewUpdates;
	QProcess *mUpdaterProcess;
	QString const mQRealPath;
	QString mUpdaterPath;

protected slots:
	void readAnswer();
};

