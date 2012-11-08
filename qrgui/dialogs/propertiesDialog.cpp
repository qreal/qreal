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
	mUi->PropertiesNamesList->setModel(new QStringListModel(propertiesDisplayedNames));
	connect(mUi->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
}

void PropertiesDialog::cancel()
{
	close();
}
