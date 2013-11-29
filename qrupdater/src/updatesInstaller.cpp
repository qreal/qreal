#include "updatesInstaller.h"

UpdatesInstaller::UpdatesInstaller(QList<Update *> updates)
{
	mUpdatesQueue.append(updates);
}

UpdatesInstaller::UpdatesInstaller()
{
}

UpdatesInstaller::operator<<(Update *update)
{
	mUpdatesQueue.append(update);
}

UpdatesInstaller::operator<<(QList<Update *> updates)
{
	mUpdatesQueue.append(updates);
}

void UpdatesInstaller::startInstalling()
{
	mHasNoErrors = true;
	if (!mUpdatesQueue.isEmpty()) {
		installNext();
	}
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
	connect(mUpdatesQueue.first(), SIGNAL(installFinished(bool)), this, SLOT(singleInstallFinished(bool)));
	mUpdatesQueue.first()->installUpdate();
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
	} else
	{
		emit installsFinished(mHasNoErrors);
	}
}
