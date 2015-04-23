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

#include "qrgui/preferencesDialog/preferencesPage.h"

namespace Ui {
	class PreferencesBehaviourPage;
}

class PreferencesBehaviourPage : public qReal::gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesBehaviourPage(QWidget *parent = 0);
	~PreferencesBehaviourPage() override;

	void save() override;
	void restoreSettings() override;

protected:
	void changeEvent(QEvent *e);

private slots:
	void showAutoSaveBox(bool show);
	void updateGesturesSettings(bool gesturesEnabled);

private:
	void initLanguages();

	Ui::PreferencesBehaviourPage *mUi;
	QString mOldLanguage;
};
