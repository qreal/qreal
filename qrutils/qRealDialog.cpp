#include "qRealDialog.h"

#include <qrkernel/settingsManager.h>

using namespace utils;
using namespace qReal;

QRealDialog::QRealDialog(QString const &id, QWidget *parent)
	: QDialog(parent)
	, mId(id)
{
}

void QRealDialog::showEvent(QShowEvent *event)
{
	QDialog::showEvent(event);
	deserializeParameters();
}

void QRealDialog::closeEvent(QCloseEvent *event)
{
	serializeParameters();
	QDialog::closeEvent(event);
}

void QRealDialog::serializeParameters()
{
	SettingsManager::setValue(maximizedKey(), isMaximized());
	SettingsManager::setValue(positionKey(), pos());
	SettingsManager::setValue(sizeKey(), size());
}

void QRealDialog::deserializeParameters()
{
	if (SettingsManager::value(maximizedKey()).toBool()) {
		setWindowState(windowState() | Qt::WindowMaximized);
	} else {
		if (SettingsManager::value(sizeKey()).toSize().isValid()) {
			move(SettingsManager::value(positionKey()).toPoint());
			resize(SettingsManager::value(sizeKey()).toSize());
		}
	}
}

QString QRealDialog::maximizedKey() const
{
	return mId + "WasMaximized";
}

QString QRealDialog::positionKey() const
{
	return mId + "LastPosition";
}

QString QRealDialog::sizeKey() const
{
	return mId + "LastSize";
}
