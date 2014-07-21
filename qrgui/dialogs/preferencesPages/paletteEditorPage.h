#pragma once

#include "dialogs/preferencesPages/preferencesPage.h"
#include "qrgui/mainwindow/palette/paletteTree.h"

#include <QtCore/QMap>

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
	QList<Id> mIdList;
	QList<QCheckBox *> *mCheckBoxList;
	EditorManagerInterface *mEditorManager;
};
