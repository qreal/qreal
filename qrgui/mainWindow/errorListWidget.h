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

#include <qrkernel/ids.h>

#include "qrgui/mainWindow/error.h"

namespace qReal {
	class MainWindow;
}

class ErrorListWidget : public QListWidget
{
	Q_OBJECT

public:
	explicit ErrorListWidget(QWidget *parent = nullptr);
	void init(qReal::MainWindow* mainWindow);

signals:
	void clearRequested();

private slots:
	void highlightElement(QListWidgetItem * const item);
	void copyCurrentItem();
	void showContextMenu(const QPoint &pos);

private:

	void initContextMenu();

	qReal::MainWindow* mMainWindow;
	QMenu *mContextMenu;
};

