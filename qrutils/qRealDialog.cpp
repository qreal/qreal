/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "qRealDialog.h"

#include <qrkernel/settingsManager.h>

using namespace utils;
using namespace qReal;

QRealDialog::QRealDialog(const QString &id, QWidget *parent)
	: QDialog(parent)
	, mId(id)
{
	deserializeParameters();
}

void QRealDialog::suspendSerialization()
{
	mSerializationSuspended = true;
}

void QRealDialog::resumeSerialization()
{
	mSerializationSuspended = false;
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
	if (mSerializationSuspended) {
		return;
	}

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
