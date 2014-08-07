#include "updatesInstaller.h"

using namespace qrUpdater;

/// Time for main program could fully unload resources
int const delayBeforeUpdate = 5000;

UpdatesInstaller::UpdatesInstaller(QList<Update *> updates)
{
	mUpdatesQueue.append(updates);
}

UpdatesInstaller::UpdatesInstaller()
{
}

UpdatesInstaller &UpdatesInstaller::operator<<(Update *update)
{
	mUpdatesQueue.append(update);
	return *this;
}

UpdatesInstaller &UpdatesInstaller::operator<<(QList<Update *> updates)
{
	mUpdatesQueue.append(updates);
	return *this;
}

void UpdatesInstaller::installAll()
{
	QTimer::singleShot(delayBeforeUpdate, this, SLOT(startInstallation()));
	sortQueue();
}

bool UpdatesInstaller::hasNoErrors() const
{
	return mHasNoErrors;
}

bool UpdatesInstaller::isEmpty() const
{
	return mUpdatesQueue.isEmpty();
}

void UpdatesInstaller::installNext()
{
	connect(mUpdatesQueue.first(), SIGNAL(installedSuccessfully(bool)), this, SLOT(singleInstallFinished(bool)));
	replaceExpressions(mUpdatesQueue.first());
	mUpdatesQueue.first()->installUpdate();
	if (mUpdatesQueue.first()->hasSelfInstallMarker()) {
		emit selfInstalling();
	}
}

void UpdatesInstaller::replaceExpressions(Update *update)
{
	typedef QString (*Function)(void);
	QMap<QString, Function> replacement;
	replacement.insert(QString("%qru:installdir%"), UpdatesInstaller::installationDirectory);

	QMutableListIterator<QString> iterator(update->arguments());
	while (iterator.hasNext()) {
		iterator.next();
		for (QString const &mask : replacement.keys()) {
			QString curKey = iterator.value();
			iterator.setValue(curKey.replace(mask, replacement.value(mask)()));
		}
	}
}

void UpdatesInstaller::sortQueue()
{
	qSort(mUpdatesQueue.begin(), mUpdatesQueue.end(), [](Update const *update1, Update const *update2) {
		// Names comparison for durability.
		return (!update1->hasSelfInstallMarker() && update2->hasSelfInstallMarker())
				|| (update1->unit() < update2->unit());
	});
}

QString UpdatesInstaller::installationDirectory()
{
	return QCoreApplication::applicationDirPath();
}

void UpdatesInstaller::singleInstallFinished(bool hasNoErrors)
{
	if (hasNoErrors) {
		if (mUpdatesQueue.first()->isInstalled()) {
			mUpdatesQueue.first()->clear();
		} else {
			mHasNoErrors = false;
		}
	}

	mUpdatesQueue.takeFirst();
	if (!mUpdatesQueue.isEmpty()) {
		installNext();
	} else {
		emit installsFinished(mHasNoErrors);
	}
}

void UpdatesInstaller::startInstallation()
{
	mHasNoErrors = true;
	if (!mUpdatesQueue.isEmpty()) {
		installNext();
	}
}
