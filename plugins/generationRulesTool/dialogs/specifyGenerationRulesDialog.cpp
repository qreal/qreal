#include "specifyGenerationRulesDialog.h"
#include "ui_specifyGenerationRulesDialog.h"

using namespace qReal;
using namespace gui;

SpecifyGenerationRulesDialog::SpecifyGenerationRulesDialog(EditorManagerInterface *interpreterEditorManager
		, const Id &id
		, qrRepo::LogicalRepoApi *metamodelRepoApi
		, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::SpecifyGenerationRulesDialog)
	, mInterpreterEditorManager(interpreterEditorManager)
	, mId(id)
	, mMetamodelRepoApi(metamodelRepoApi)
{
	mUi->setupUi(this);

	addPropertiesList();
	addOldRule();

	connect(mUi->propertiesView, &QListWidget::itemDoubleClicked, this
			, &SpecifyGenerationRulesDialog::insertPropertyIntoCode);

	connect(mUi->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this
			, &SpecifyGenerationRulesDialog::saveGenerationRule);

	this->setVisible(true);
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

void SpecifyGenerationRulesDialog::saveGenerationRule()
{
	mInterpreterEditorManager->updateGenerationRule(mId, mUi->codeArea->toPlainText());
}

void SpecifyGenerationRulesDialog::addPropertiesList()
{
	mPropertiesNames = mInterpreterEditorManager->propertyNames(mId);
	const QStringList propertiesDisplayedNames = propertiesDisplayedNamesList(mPropertiesNames);
	mUi->propertiesView->clear();
	mUi->propertiesView->addItems(propertiesDisplayedNames);
}

void SpecifyGenerationRulesDialog::addOldRule()
{
	const QString previousRule = mInterpreterEditorManager->generationRule(mId);
	mUi->codeArea->insertPlainText(previousRule);
}

QStringList SpecifyGenerationRulesDialog::propertiesDisplayedNamesList(const QStringList &propertiesNames)
{
	QStringList propertiesDisplayedNames;
	for (const QString &propertyName : propertiesNames) {
		propertiesDisplayedNames << mInterpreterEditorManager->propertyDisplayedName(mId, propertyName);
	}

	return propertiesDisplayedNames;
}
