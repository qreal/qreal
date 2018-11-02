/* Copyright 2007-2105 QReal Research Group, Dmitry Mordvinov
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

#include <QtWidgets/QTabWidget>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/editorInterface.h>

namespace qReal {
namespace gui {

/// Custom class for showing tabs in main window.
class TabWidget : public QTabWidget, public EditorInterface
{
	Q_OBJECT

public:
	/// Constructor.
	explicit TabWidget(QWidget *parent = nullptr);

	/// Hides close button on the tab with the given \a index.
	void setTabUnclosable(int index);

	QString editorId() const override;

	bool supportsZooming() const override;
	bool supportsCopying() const override;
	bool supportsPasting() const override;
	bool supportsCutting() const override;
	bool supportsSearching() const override;
	bool supportsReplacingBy() const override;

	void zoomIn() override;
	void zoomOut() override;

	void copy() override;
	void paste() override;
	void cut() override;
	void find() override;
	void replaceBy() override;

	void forceFocus() override;

	/// Overrides tab widget behaviour configuring editors with actions.
	int addTab(QWidget *widget, const QString &name);

protected:
	void mousePressEvent(QMouseEvent *event) override;

private:
	EditorInterface *currentEditor() const;
	void onTabChanged();
};

}
}
