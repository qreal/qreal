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

#include <QtWidgets/QFontDialog>

#include "qrgui/preferencesDialog/preferencesPage.h"

namespace Ui {
	class PreferencesEditorPage;
}

class PreferencesEditorPage : public qReal::gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesEditorPage(QWidget *parent = 0);
	~PreferencesEditorPage();

	/// Sets value to palette combo box and spin box respectively/
	void changePaletteParameters();
	void save();
	virtual void restoreSettings();

protected:
	void changeEvent(QEvent *e);

private slots:
	void widthGridSliderMoved(int value);
	void indexGridSliderMoved(int value);
	void dragAreaSliderMoved(int value);
	void manualFontCheckBoxChecked(bool);
	void fontSelectionButtonClicked();
	void paletteComboBoxClicked(int index);

private:
	Ui::PreferencesEditorPage *mUi;
	int mWidthGrid;
	int mIndexGrid;
	int mDragArea;
	QString mFont;

	bool mFontButtonWasPressed;
	bool mWasChecked;
	QString mOldFont;
};
