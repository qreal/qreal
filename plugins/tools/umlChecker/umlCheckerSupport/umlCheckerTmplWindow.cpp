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

#include "umlCheckerTmplWindow.h"
#include "ui_umlCheckerTmplWindow.h"
#include <QtCore/QDir>

using namespace qReal;

UmlCheckerTmplWindow::UmlCheckerTmplWindow(QWidget *parent)
		: QDialog(parent)
		, mUi(new Ui::umlCheckerForm)
{
//	mUi->setupUi(this);
//	mUi->saveButton->setEnabled(true);
////	mUi->canselButton->setEnabled(true);


//	connect(mUi->listView, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(openPicture(QListWidgetItem*)));
//	connect(mUi->saveButton, SIGNAL(checkClicked()), this, SLOT(checkActivate()));
}


UmlCheckerTmplWindow::~UmlCheckerTmplWindow()
{

}


void UmlCheckerTmplWindow::checkActivate()
{
	emit checkClicked();
}
