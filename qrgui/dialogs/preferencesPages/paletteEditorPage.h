#pragma once

#include "dialogs/preferencesPages/preferencesPage.h"
#include "qrgui/mainwindow/palette/paletteTree.h"

#include <QtCore/QMap>

namespace Ui {
class PreferencesPaletteEditorPage;
}

/// palette preferences - use or not elements.
class PreferencesPaletteEditorPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesPaletteEditorPage(QWidget *parent, qReal::EditorManagerInterface *editorManager);
	~PreferencesPaletteEditorPage();

	void save() override;
	virtual void restoreSettings();

private slots:
	/// select all checkBoxes
	void selectAll();

	/// unselect all checkBoxes
	void unselectAll();

private:
	/// init check boxes, icons and labels.
	void initCheckBoxes(QList<QList<QString> > listOfIdString);

	Ui::PreferencesPaletteEditorPage *mUi;

	/// all groups with all Id
	QList<QList<Id>> mIdList;

	/// checkBoxes for check use or not elements.
	QList<QCheckBox *> *mCheckBoxList;

	/// editor manager.
	EditorManagerInterface *mEditorManager;
};
