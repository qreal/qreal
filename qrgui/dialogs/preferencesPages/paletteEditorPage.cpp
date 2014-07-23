#include "paletteEditorPage.h"
#include "ui_paletteEditorPage.h"

#include <QtWidgets/QScrollBar>
#include <QtWidgets/QScrollArea>

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

	QList<QList<QString>> listOfIdString;
	for (Id const &editor : editorManager->editors()) {
		for (Id const &diagram : editorManager->diagrams(editor)) {
			for (QString group : editorManager->paletteGroups(editor, diagram)) {
				listOfIdString.append(editorManager->paletteGroupList(editor, diagram, group));
				mIdList.append(QList<Id>());
			}
		}
	}

	int i = 0;
	int j = 0;

	for (Id element : editorManager->getAllIP()) {
		j = 0;
		for (QList<QString> temp : listOfIdString){
			if (temp.contains(element.element()))
				mIdList[j].append(element);
			j++;
		}
	}

	for (QList<Id> const &group : mIdList) {
		for (Id const &element : group) {
			QLabel *label = new QLabel;
			QCheckBox *box = new QCheckBox;
			QIcon icon = editorManager->icon(element);
			box->setChecked(true);
			mCheckBoxList->append(box);
			label->setPixmap(icon.pixmap(QSize(30, 30)));
			mUi->gridLayout_2->addWidget(box, i, 0);
			mUi->gridLayout_2->addWidget(label, i, 1);
			mUi->gridLayout_2->addWidget(new QLabel(editorManager->friendlyName(element)), i, 2, 1, 10);
			++i;
		}
	}

	QScrollArea *scrollArea = new QScrollArea(this);
	scrollArea->setWidget(mUi->widget);
	mUi->widget->show();
	scrollArea->show();
	mUi->gridLayout->addWidget(scrollArea);

//	QScrollBar scrollBar = QScrollBar(Qt::Vertical, this);
}

PreferencesPaletteEditorPage::~PreferencesPaletteEditorPage()
{
	delete mUi;
	delete mCheckBoxList;
	delete mEditorManager;
}

void PreferencesPaletteEditorPage::save()
{
	int i = 0;
	for (QList<Id> diagram : mIdList) {
		for (Id element : diagram) {
			mEditorManager->setElementEnabled(element, mCheckBoxList->at(i)->isChecked());
			SettingsManager::setValue(element.toString(), mCheckBoxList->at(i)->isChecked());
			i++;
		}
	}
}

void PreferencesPaletteEditorPage::restoreSettings()
{
	int i = 0;
	for (QList<Id> diagram : mIdList) {
		for (Id element : diagram) {
			mEditorManager->setElementEnabled(element, SettingsManager::value(element.toString()).toBool());
			mCheckBoxList->at(i)->setChecked(SettingsManager::value(element.toString()).toBool());
			i++;
		}
	}
}

