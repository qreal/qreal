#pragma once

#include <QtCore/QStringList>
#include <QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>

#include "../qrkernel/settingsManager.h"

//! @class QRealUpdater starts and interacts with Updater
//! it tells qreal dont to load GUI and quit immediatly to make update
class QRealUpdater : public QObject
{
	Q_OBJECT
public:
	explicit QRealUpdater(char const *applicationPath);

	//! @return True if there are some updates to intall
	bool hasUpdates();

	//! starts separate Updater program
	void startUpdater();

protected:
	bool hasUpdatePermission();
	void executeUpdater();
	void transferInfo();
	QStringList collectArguments();

	void prepareForClose();

	static int const updaterTimeout = 1000;
	bool mHasNewUpdates;
	QProcess *mUpdaterProcess;
	QString const mQRealPath;
	QString mUpdaterPath;

protected slots:
	void readAnswer();
};

