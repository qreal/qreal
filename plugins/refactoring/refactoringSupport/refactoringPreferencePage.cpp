#include <QFileDialog>

#include "refactoringPreferencePage.h"
#include "ui_refactoringPreferencePage.h"

#include "../../../qrkernel/settingsManager.h"

using namespace qReal;

RefactoringPreferencesPage::RefactoringPreferencesPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::refactoringPreferencesPage)
{
	mIcon = QIcon(":/icons/preferences/pencil.png");
	mUi->setupUi(this);

	QString binFolder = qApp->applicationDirPath();

	mUi->qrealSourcesLineEdit->setText(SettingsManager::value("qrealSourcesLocation"
			, binFolder.mid(0, binFolder.lastIndexOf("/"))).toString());

	connect(mUi->qrealSourcesPushButton, SIGNAL(clicked()), this, SLOT(setQRealSourcesLocation()));
	connect(mUi->linuxRadioButton, SIGNAL(clicked()), this, SLOT(changeDefaultDotPath()));
	connect(mUi->windowsRadioButton, SIGNAL(clicked()), this, SLOT(changeDefaultDotPath()));
	connect(mUi->otherRadioButton, SIGNAL(clicked()), this, SLOT(changeDefaultDotPath()));
	connect(mUi->browseDotPathPushButton, SIGNAL(clicked()), this, SLOT(setDotPathManually()));

	mUi->colorComboBox->addItems(QColor::colorNames());
	QString curColor = SettingsManager::value("refactoringColor", "cyan").toString();
	int curColorIndex = mUi->colorComboBox->findText(curColor);
	mUi->colorComboBox->setCurrentIndex(curColorIndex);


	mUi->windowsRadioButton->setChecked(SettingsManager::value("dotWindowsChecked").toBool());
	mUi->linuxRadioButton->setChecked(SettingsManager::value("dotLinuxChecked").toBool());
	mUi->otherRadioButton->setChecked(SettingsManager::value("dotOtherChecked").toBool());
	mUi->dotPathLineEdit->setText(SettingsManager::value("pathToDot").toString());
	
	if (SettingsManager::value("dotOtherChecked").toBool()) {
		mUi->dotPathLineEdit->setEnabled(true);
		mUi->browseDotPathPushButton->setEnabled(true);
	}
}

RefactoringPreferencesPage::~RefactoringPreferencesPage()
{
	delete mUi;
}

void RefactoringPreferencesPage::setQRealSourcesLocation()
{
	QString location = QFileDialog::getExistingDirectory(this, tr("Specify directory:"));
	mUi->qrealSourcesLineEdit->setText(location);
}

void RefactoringPreferencesPage::save()
{
	SettingsManager::setValue("qrealSourcesLocation", mUi->qrealSourcesLineEdit->text());
	SettingsManager::setValue("refactoringColor", mUi->colorComboBox->currentText());
	SettingsManager::setValue("dotWindowsChecked", mUi->windowsRadioButton->isChecked());
	SettingsManager::setValue("dotLinuxChecked", mUi->linuxRadioButton->isChecked());
	SettingsManager::setValue("dotOtherChecked", mUi->otherRadioButton->isChecked());
	SettingsManager::setValue("pathToDot", mUi->dotPathLineEdit->text());
}

void RefactoringPreferencesPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void RefactoringPreferencesPage::changeDefaultDotPath()
{
	mUi->dotPathLineEdit->setEnabled(mUi->otherRadioButton->isChecked());
	mUi->browseDotPathPushButton->setEnabled(mUi->otherRadioButton->isChecked());
	
	if (mUi->windowsRadioButton->isChecked()) {
		mUi->dotPathLineEdit->setText("dot.exe");
	}
	if (mUi->linuxRadioButton->isChecked()) {
		mUi->dotPathLineEdit->setText("dot");
	}
	if (mUi->otherRadioButton->isChecked()) {
		mUi->dotPathLineEdit->setText("");
	}
}

void RefactoringPreferencesPage::setDotPathManually()
{
	QString const path = QFileDialog::getOpenFileName(this, tr("Specify file:"));
	mUi->dotPathLineEdit->setText(path);
}
