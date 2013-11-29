#pragma once

#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtCore/QProcess>

#include "update.h"

//! @class UpdatesInstaller installs queue of updates
class UpdatesInstaller : public QObject
{
	Q_OBJECT
public:
	explicit UpdatesInstaller(QList<Update *> updates);
	UpdatesInstaller();
	operator<< (Update *update);
	operator<< (QList<Update *> updates);

	//! installes all of updates in queue
	void startInstalling();

	bool hasNoErrors() const;
	bool isEmpty() const;

signals:
	void installsFinished(bool hasNoErrors);


protected:
	void installNext();

	bool mHasNoErrors;
	QQueue<Update *> mUpdatesQueue;

protected slots:
	void singleInstallFinished(bool hasNoErrors);

};

