#include "updateVersionDialog.h"

#include <QtWidgets/QMessageBox>

#include <qrkernel/settingsManager.h>

using namespace qReal;

QString UpdateVersionDialog::readyMessage(QString const &version)
{
	return QObject::tr("A new version %1 is ready to be installed. Select one of the actions below.").arg(version);
}

QString UpdateVersionDialog::availableMessage(QString const &version)
{
	return QObject::tr("A new version %1 is available. Select one of the actions below.").arg(version);
}

bool UpdateVersionDialog::promptUpdate(QString const &newVersion, QWidget *parent)
{
	if (isVersionDeclined(newVersion)) {
		return false;
	}

	QString const message = SettingsManager::value("downloadUpdates").toBool()
			? readyMessage(newVersion)
			: availableMessage(newVersion);

	QString const updateButtonText = SettingsManager::value("downloadUpdates").toBool()
			? QObject::tr("Update")
			: QObject::tr("Download and update");

	QString const laterButtonText = QObject::tr("Later");
	QString const forgetButtonText = QObject::tr("Forget");

	int const button = QMessageBox::information(parent, QObject::tr("Yeah!"), message
			, updateButtonText, laterButtonText, forgetButtonText, 0, 1);
	if (button == 2) {
		// Forgetting...
		declineVersion(newVersion);
	}

	return button == 0;
}

void UpdateVersionDialog::declineVersion(QString const &version)
{
	SettingsManager::setValue(declineSettingsKey(version), true);
}

bool UpdateVersionDialog::isVersionDeclined(QString const &version)
{
	return SettingsManager::value(declineSettingsKey(version)).toBool();
}

QString UpdateVersionDialog::declineSettingsKey(QString const &version)
{
	return QString("updateTo%1Declined").arg(version);
}
