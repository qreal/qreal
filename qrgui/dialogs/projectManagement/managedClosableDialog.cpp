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

#include "managedClosableDialog.h"

#include <QtGui/QtEvents>

using namespace qReal;

ManagedClosableDialog::ManagedClosableDialog(QWidget *parent, bool isClosable)
	: QDialog(parent, isClosable ? Qt::Dialog : Qt::WindowMinimizeButtonHint)
	, mIsClosable(isClosable)
{
}

void ManagedClosableDialog::setClosability(bool isClosable)
{
	mIsClosable = isClosable;
}

bool ManagedClosableDialog::forceClose()
{
	setClosability(true);
	return close();
}

void ManagedClosableDialog::closeEvent(QCloseEvent *event)
{
	event->setAccepted(mIsClosable);
}

void ManagedClosableDialog::keyPressEvent(QKeyEvent *event)
{
	if (!mIsClosable && event->key() == Qt::Key_Escape) {
		event->ignore();
		return;
	}
	QDialog::keyPressEvent(event);
}
