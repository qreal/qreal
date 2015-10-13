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

#include <QtWidgets/QLineEdit>

#include <qrgui/preferencesDialog/preferencesPage.h>

namespace Ui {
	class VisualDebuggerPreferencesPage;
}

namespace qReal {

/// Preference page for visual debugger of block diagrams (also with gdb debug)
class VisualDebuggerPreferencesPage : public gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit VisualDebuggerPreferencesPage(QWidget *parent = 0);
	~VisualDebuggerPreferencesPage();

	void save();
	virtual void restoreSettings();
private slots:

	/// Set path to builder (like gcc)
	void setBuilderPath();

	/// Set path to debugger (like gdb)
	void setDebuggerPath();

	/// Set working directory path in which the source code will be generated
	/// and executable will be builded
	void setWorkDir();

private:

	/// Show choose path dialog
	QString choosePath(bool isFolder);
	void putTextInLineEdit(QLineEdit *lineEdit, QString const &text);

	Ui::VisualDebuggerPreferencesPage *mUi;
};

}
