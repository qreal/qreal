#include <QFileDialog>

#include "refactoringPreferencePage.h"
#include "ui_refactoringPreferencePage.h"

#include "../../../qrkernel/settingsManager.h"

using namespace qReal;

RefactoringPreferencesPage::RefactoringPreferencesPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::refactoringPreferencesPage)
{
	mIcon = QIcon(":/icons/preferences/pencil.png");
	mUi->setupUi(this);

	QString binFolder = qApp->applicationDirPath();

	mUi->qrealSourcesLineEdit->setText(SettingsManager::value("qrealSourcesLocation",
			binFolder.mid(0, binFolder.lastIndexOf("/"))).toString());

	connect(mUi->qrealSourcesPushButton, SIGNAL(clicked()), this, SLOT(setQRealSourcesLocation()));
}

RefactoringPreferencesPage::~RefactoringPreferencesPage() {
	delete mUi;
}

void RefactoringPreferencesPage::setQRealSourcesLocation() {
	QString location = QFileDialog::getExistingDirectory(this, tr("Specify directory:"));
	mUi->qrealSourcesLineEdit->setText(location);
}

void RefactoringPreferencesPage::save() {
	SettingsManager::setValue("qrealSourcesLocation", mUi->qrealSourcesLineEdit->text());
}
