#include <QStringListModel>
#include "propertiesDialog.h"
#include "ui_propertiesDialog.h"

using namespace qReal;

PropertiesDialog::PropertiesDialog(QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::PropertiesDialog)
{
	mUi->setupUi(this);
	setWindowTitle(tr("Properties"));
}

PropertiesDialog::~PropertiesDialog()
{
	delete mUi;
}

void PropertiesDialog::init(EditorManagerInterface* interperterEditorManager, Id const &id)
{
	QStringList propertiesNames = interperterEditorManager->getPropertyNames(id);
	QStringList propertiesDisplayedNames;
	foreach (QString propertyName, propertiesNames) {
		propertiesDisplayedNames << interperterEditorManager->propertyDisplayedName(id, propertyName);
	}
	mUi->PropertiesNamesList->addItems(propertiesDisplayedNames);
	mUi->PropertiesNamesList->setWrapping(true);
	int size = propertiesNames.length();
	for (int i = 0; i < size; i++) {
		if (interperterEditorManager->isParentProperty(id, propertiesNames[i])) {
			mUi->PropertiesNamesList->findItems(propertiesDisplayedNames[i], Qt::MatchFixedString).first()->setFlags(Qt::NoItemFlags);
		}
	}

	connect(mUi->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
}

void PropertiesDialog::cancel()
{
	close();
}
