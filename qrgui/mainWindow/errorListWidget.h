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
#include <QtWidgets/QLabel>
#include <QtWidgets/QAction>

#include <qrkernel/ids.h>
#include <qrgui/mainWindow/error.h>
#include <qrutils/outputWidgets/outputWidget.h>

namespace qReal {
namespace gui {

class ErrorListWidget : public utils::OutputWidget
{
	Q_OBJECT

public:
	explicit ErrorListWidget(QWidget *parent = nullptr);

	int count() const;
	void clear();
	void addError(const Error &error);

	QString title() const override;

	QAction *action() override;

	QString shortcutName() const override;

signals:
	void clearRequested();

	void highlightId(const Id &id);

private slots:
	void highlightElement(QListWidgetItem * const item);
	void copyCurrentItem();
	void showContextMenu(const QPoint &pos);

private:
	static QString severityMessage(const Error &error);
	static QIcon errorIcon(const Error &error);

	void initContextMenu();

	QListWidget mListWidget;
	QMenu *mContextMenu;
	QAction mAction;
};

}
}
