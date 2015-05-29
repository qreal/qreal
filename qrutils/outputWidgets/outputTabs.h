/* Copyright 2015 QReal Research Group
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

#include <qrutils/outputWidgets/outputWidget.h>

namespace utils {

/// Tabs widgets that manages displaying of different output widgets.
class OutputTabs : public QTabWidget
{
	Q_OBJECT
public:
	explicit OutputTabs(QWidget *parent = 0);
	~OutputTabs();

	/// Takes control over given widget.
	void addWidget(OutputWidget *widget);

	/// Sets a dock to contol its visibility.
	void setDock(QWidget *dock);

public slots:
	/// Changes visibility of current widget. If the current widget is shown, closes dock.
	void toggleVisibility();

	/// Opens tab which contains a sender widget.
	void showOutputWidget();

	/// Closes tab which contains a sender widget.
	void hideOutputWidget();

private slots:
	void closeTab(int index);

private:
	void showWidget(OutputWidget *outputWidget);

	QList<OutputWidget *> mWidgets;  // Has ownership
	QWidget *mDock;  // Doesn't have ownership
};

}
