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

#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidgetItem>
#include <QtCore/QModelIndex>

#include <qrkernel/ids.h>

#include "models/models.h"

namespace Ui
{
class ReferenceList;
}

namespace qReal
{
class MainWindow;
}

/** @brief ReferenceList is a widget which allows to edit reference properties
*
* Shows all the logical elements of needed type and sets reference property to chosen value
*/

class ReferenceList : public QDialog
{
	Q_OBJECT

public:
	ReferenceList(qReal::MainWindow *mainWindow, const QPersistentModelIndex &index
			, const QString &refType, const QStringList &currentValue, int role, QWidget *parent = 0);
	~ReferenceList();

public slots:
	void activateElement(QListWidgetItem *);

signals:
	/// emitted when OK pressed
	void referenceSet(const QStringList &newValue, const QPersistentModelIndex &index, int role);

private slots:
	void valueChanged();
	void restoreSelected();

private:
	void loadList(const QString &refType);
	void addItem(const qReal::Id &element);

	void highlightCurrentValue(const QStringList &currentValue);
	QStringList getNewValue() const;

	void initConnections();

	Ui::ReferenceList *mUi;

	QPersistentModelIndex mIndex;
	int mRole;

	qReal::MainWindow *mWindow;
};
