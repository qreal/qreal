/* Copyright 2017 CyberTech Labs Ltd
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

#include "qRealMessageBox.h"

#include <QtCore/QMap>
#include <QDebug>

QMessageBox::StandardButton utils::QRealMessageBox::question(QWidget *parent
		, const QString &title
		, const QString &text
		, QMessageBox::StandardButtons buttons)
{
	QMessageBox *messageBox = new QMessageBox(parent);
	messageBox->setWindowTitle(title);
	messageBox->setText(text);
	messageBox->setStandardButtons(buttons);

	if (buttons & QMessageBox::Ok) { messageBox->setButtonText(QMessageBox::Ok, tr("Ok")); }
	if (buttons & QMessageBox::Open) { messageBox->setButtonText(QMessageBox::Open, tr("Open")); }
	if (buttons & QMessageBox::Save) { messageBox->setButtonText(QMessageBox::Save, tr("Save")); }
	if (buttons & QMessageBox::Cancel) { messageBox->setButtonText(QMessageBox::Cancel, tr("Cancel")); }
	if (buttons & QMessageBox::Close) { messageBox->setButtonText(QMessageBox::Close, tr("Close")); }
	if (buttons & QMessageBox::Discard) { messageBox->setButtonText(QMessageBox::Discard, tr("Discard")); }
	if (buttons & QMessageBox::Apply) { messageBox->setButtonText(QMessageBox::Apply, tr("Apply")); }
	if (buttons & QMessageBox::Reset) { messageBox->setButtonText(QMessageBox::Reset, tr("Reset")); }
	if (buttons & QMessageBox::Help) { messageBox->setButtonText(QMessageBox::Help, tr("Help")); }
	if (buttons & QMessageBox::SaveAll) { messageBox->setButtonText(QMessageBox::SaveAll, tr("Save All")); }
	if (buttons & QMessageBox::Yes) { messageBox->setButtonText(QMessageBox::Yes, tr("Yes")); }
	if (buttons & QMessageBox::YesToAll) { messageBox->setButtonText(QMessageBox::YesToAll, tr("Yes To All")); }
	if (buttons & QMessageBox::No) { messageBox->setButtonText(QMessageBox::No, tr("No")); }
	if (buttons & QMessageBox::NoToAll) { messageBox->setButtonText(QMessageBox::NoToAll, tr("No To All")); }
	if (buttons & QMessageBox::Abort) { messageBox->setButtonText(QMessageBox::Abort, tr("Abort")); }
	if (buttons & QMessageBox::Retry) { messageBox->setButtonText(QMessageBox::Retry, tr("Retry")); }
	if (buttons & QMessageBox::Ignore) { messageBox->setButtonText(QMessageBox::Ignore, tr("Ignore")); }
	if (buttons & QMessageBox::NoButton) { messageBox->setButtonText(QMessageBox::NoButton, tr("NoButton")); }
	if (buttons & QMessageBox::RestoreDefaults) {
		messageBox->setButtonText(QMessageBox::RestoreDefaults, tr("Restore Defaults"));
	}

	auto result = static_cast<QMessageBox::StandardButton>(messageBox->exec());
	if (!parent) {
		delete messageBox;
	}

	return result;
}
