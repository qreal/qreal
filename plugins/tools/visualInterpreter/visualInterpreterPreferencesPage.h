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
	class visualInterpreterPreferencesPage;
}

namespace qReal {

/// Preferences page for visual interpreter. Seems to be more or less useless rigth now.
class VisualInterpreterPreferencesPage : public gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit VisualInterpreterPreferencesPage(QWidget *parent = 0);
	~VisualInterpreterPreferencesPage();

	void save();
	void restoreSettings();
private slots:
	void setQRealSourcesLocation();
	void setPythonPath();
	void setTempScriptPath();

private:
	Ui::visualInterpreterPreferencesPage *mUi;
};

}

