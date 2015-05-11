#include "specifyPathToGeneratedCodeDialog.h"
#include "ui_specifyPathToGeneratedCodeDialog.h"

using namespace qReal::gui;

SpecifyPathToGeneratedCodeDialog::SpecifyPathToGeneratedCodeDialog(QWidget *parent) :
		QDialog(parent),
		mUi(new Ui::SpecifyPathToGeneratedCodeDialog)
{
	mUi->setupUi(this);
	this->setVisible(true);
}

SpecifyPathToGeneratedCodeDialog::~SpecifyPathToGeneratedCodeDialog()
{
	delete mUi;
}

void SpecifyPathToGeneratedCodeDialog::restoreSettings()
{

}

void SpecifyPathToGeneratedCodeDialog::saveSettings()
{

}

QString SpecifyPathToGeneratedCodeDialog::currentFileName() const
{

}

QString SpecifyPathToGeneratedCodeDialog::currentPathToFolder() const
{

}
