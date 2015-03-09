#include "specifyGenerationRulesDialog.h"
#include "ui_specifyGenerationRulesDialog.h"

using namespace qReal;
using namespace gui;

SpecifyGenerationRulesDialog::SpecifyGenerationRulesDialog(MainWindow &mainWindow
		, EditorManagerInterface &interpreterEditorManager
		, Id const &id) :
	QDialog(&mainWindow)
	, mUi(new Ui::SpecifyGenerationRulesDialog)
	, mInterpreterEditorManager(interpreterEditorManager)
	, mId(id)
{
	mUi->setupUi(this);

	addPropertiesList();
	connect(mUi->propertiesView, &QListWidget::itemDoubleClicked, this
			&SpecifyGenerationRulesDialog::insertPropertyIntoCode);
}

SpecifyGenerationRulesDialog::~SpecifyGenerationRulesDialog()
{
	delete mUi;
}

void SpecifyGenerationRulesDialog::insertPropertyIntoCode(QListWidgetItem* property)
{
	QString const propertyName = property->text();
	mUi->codeArea->insertPlainText(propertyName);
}

void SpecifyGenerationRulesDialog::addPropertiesList()
{
	mPropertiesNames = mInterpreterEditorManager.propertyNames(mId);
	QStringList const propertiesDisplayedNames = propertiesDisplayedNamesList(mPropertiesNames);
	mUi->propertiesView->clear();
	mUi->propertiesView->addItems(propertiesDisplayedNames);
}

QStringList SpecifyGenerationRulesDialog::propertiesDisplayedNamesList(QStringList const &propertiesNames)
{
	QStringList propertiesDisplayedNames;
	for (QString const &propertyName : propertiesNames) {
		propertiesDisplayedNames << mInterpreterEditorManager.propertyDisplayedName(mId, propertyName);
	}

	return propertiesDisplayedNames;
}
