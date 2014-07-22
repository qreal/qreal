#include "paletteEditorPage.h"
#include "ui_paletteEditorPage.h"

#include <qrkernel/settingsManager.h>

using namespace qReal;
using namespace gui;

PreferencesPaletteEditorPage::PreferencesPaletteEditorPage(
		QWidget *parent, EditorManagerInterface *editorManager)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesPaletteEditorPage)
{
	mUi->setupUi(this);

	mEditorManager = editorManager;
	mCheckBoxList = new QList<QCheckBox *>;
	mIdList = mEditorManager->getAllIP();
	int i = 0;
	for (Id const &element : mIdList) {
		QLabel *label = new QLabel;
		QCheckBox *box = new QCheckBox;
		QIcon icon = editorManager->icon(element);

		box->setChecked(true);
		mCheckBoxList->append(box);
		label->setPixmap(icon.pixmap(QSize(30, 30)));
		mUi->gridLayout->addWidget(box, i, 0);
		mUi->gridLayout->addWidget(label, i, 1);
		mUi->gridLayout->addWidget(new QLabel(editorManager->friendlyName(element)), i, 2, 1, 10);
		i++;
	}
}

PreferencesPaletteEditorPage::~PreferencesPaletteEditorPage()
{
	delete mUi;
	delete mCheckBoxList;
	delete mEditorManager;
}

void PreferencesPaletteEditorPage::save()
{
	for (int i = 0; i < mCheckBoxList->length(); i++) {
		mEditorManager->setElementEnabled(mIdList.at(i), mCheckBoxList->at(i)->isChecked());
		if (mCheckBoxList->at(i)->isChecked())
			SettingsManager::setValue(mIdList.at(i).toString(), "true");
		else
			SettingsManager::setValue(mIdList.at(i).toString(), "false");
	}
}

void PreferencesPaletteEditorPage::restoreSettings()
{
	for (int i = 0; i < mCheckBoxList->length(); i++) {
		mEditorManager->setElementEnabled(mIdList.at(i)
										  , SettingsManager::value(mIdList.at(i).toString()).toBool());
		mCheckBoxList->at(i)->setChecked(SettingsManager::value(mIdList.at(i).toString()).toBool());
	}

}
