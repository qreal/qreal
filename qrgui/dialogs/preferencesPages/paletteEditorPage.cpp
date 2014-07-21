#include "paletteEditorPage.h"
#include "ui_paletteEditorPage.h"
#include "qrgui/mainwindow/palette/paletteTree.h"

#include <qrkernel/settingsManager.h>
using namespace qReal;
using namespace gui;

PreferencesPaletteEditorPage::PreferencesPaletteEditorPage(
		QWidget *parent, EditorManagerInterface *editorManager)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesPaletteEditorPage)
{
	mUi->setupUi(this);

	QList<Id> id;
	for (Id const &editor : editorManager->editors()) {
		for (Id const &diagram : editorManager->diagrams(editor)) {
			for (Id const &element : editorManager->elements(diagram)) {
				id.append(element);
				//mUi->listWidget->addItem(QCheckBox());
				mUi->listWidget->addItem(new QListWidgetItem(editorManager->icon(element)
														 , editorManager->friendlyName(element)));
			}
		}
	}

}

PreferencesPaletteEditorPage::~PreferencesPaletteEditorPage()
{
	delete mUi;
}

void PreferencesPaletteEditorPage::save()
{

}

void PreferencesPaletteEditorPage::restoreSettings()
{

}
