#include "specifyGenerationRulesDialog.h"
#include "ui_specifyGenerationRulesDialog.h"

using namespace qReal;
using namespace gui;

SpecifyGenerationRulesDialog::SpecifyGenerationRulesDialog(MainWindow &mainWindow
		, const EditorManagerInterface &interpreterEditorManager
		, Id const &id)
	: QDialog(&mainWindow)
	, mUi(new Ui::SpecifyGenerationRulesDialog)
	, mInterpreterEditorManager(interpreterEditorManager)
	, mId(id)
{
	mUi->setupUi(this);

	addPropertiesList();
	connect(mUi->propertiesView, &QListWidget::itemDoubleClicked, this
			, &SpecifyGenerationRulesDialog::insertPropertyIntoCode);
}

SpecifyGenerationRulesDialog::~SpecifyGenerationRulesDialog()
{
	delete mUi;
}

void SpecifyGenerationRulesDialog::insertPropertyIntoCode(QListWidgetItem* property)
{
	const QString propertyName = property->text();
	mUi->codeArea->insertPlainText(propertyName);
}

void SpecifyGenerationRulesDialog::addPropertiesList()
{
	mPropertiesNames = mInterpreterEditorManager.propertyNames(mId);
	const QStringList propertiesDisplayedNames = propertiesDisplayedNamesList(mPropertiesNames);
	mUi->propertiesView->clear();
	mUi->propertiesView->addItems(propertiesDisplayedNames);
}

QStringList SpecifyGenerationRulesDialog::propertiesDisplayedNamesList(const QStringList &propertiesNames)
{
	QStringList propertiesDisplayedNames;
	for (const QString &propertyName : propertiesNames) {
		propertiesDisplayedNames << mInterpreterEditorManager.propertyDisplayedName(mId, propertyName);
	}

	return propertiesDisplayedNames;
}
