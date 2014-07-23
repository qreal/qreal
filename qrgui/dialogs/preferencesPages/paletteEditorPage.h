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

	void save() override;
	virtual void restoreSettings();

private slots:
	void selectAll();
	void unselectAll();

private:
	// init check boxes, icons and labels.
	void initCheckBoxes(QList<QList<QString> > listOfIdString);

	Ui::PreferencesPaletteEditorPage *mUi;
	QList<QList<Id>> mIdList;
	QList<QCheckBox *> *mCheckBoxList;
	EditorManagerInterface *mEditorManager;
};
