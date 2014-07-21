#pragma once

#include "dialogs/preferencesPages/preferencesPage.h"

namespace Ui {
class PreferencesPaletteEditorPage;
}

class PreferencesPaletteEditorPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesPaletteEditorPage(QWidget *parent, qReal::EditorManagerInterface *editorManager);
	~PreferencesPaletteEditorPage();

	void save();
	virtual void restoreSettings();

private:
	Ui::PreferencesPaletteEditorPage *mUi;
};
