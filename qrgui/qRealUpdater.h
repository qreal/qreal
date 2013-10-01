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
	explicit QRealUpdater(char *argv[]);

	bool hasUpdates();

	void startUpdater();

protected:
	void executeUpdater();

	bool mHasAnswer;
	bool mHasNewUpdates;
	QProcess *mUpdaterProcess;
	QString const mQRealPath;
	QString mUpdaterPath;

protected slots:
	void readAnswer();

};

