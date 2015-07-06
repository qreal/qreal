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

#include <QtWidgets/QListWidget>

namespace qReal {

/// @brief List with button for confirm choice. The selected data is sent through a signal.
/// List in which you can add element together with associated with it the string data.
/// Selecting an item by either double clicking or by using the supporting buttons.
/// The selected string data sent through the signal userDataSelected(const QString &userData).
class ListWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ListWidget(QWidget *parent = 0);

	/// @param text Item label
	/// @param userData Associated with item the string data
	/// @param toolTip Item tooltip
	void addItem(const QString &text, const QString &userData, const QString &toolTip = "");
	void highlightFirstItem();

signals:
	void userDataSelected(const QString &userData);

private slots:
	void doubleClickHandler(QListWidgetItem *item);

private:
	QString userData(QListWidgetItem *item);

protected:
	QListWidget *mListWidget;
};

}
