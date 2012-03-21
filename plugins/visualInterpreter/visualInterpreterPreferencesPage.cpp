#include <QFileDialog>

#include "visualInterpreterPreferencesPage.h"
#include "ui_visualInterpreterPreferencePage.h"

#include "../../../qrkernel/settingsManager.h"

using namespace qReal;

VisualInterpreterPreferencesPage::VisualInterpreterPreferencesPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::visualInterpreterPreferencesPage)
{
	mIcon = QIcon(":/icons/preferences/bug.png");
	mUi->setupUi(this);
	
	QString binFolder = qApp->applicationDirPath();
	
	mUi->qrealSourcesLineEdit->setText(SettingsManager::value("qrealSourcesLocation",
			binFolder.mid(0, binFolder.lastIndexOf("/"))).toString());
	
	connect(mUi->qrealSourcesPushButton, SIGNAL(clicked()), this, SLOT(setQRealSourcesLocation()));
}

VisualInterpreterPreferencesPage::~VisualInterpreterPreferencesPage() {
	delete mUi;
}

void VisualInterpreterPreferencesPage::setQRealSourcesLocation() {
	QString location = QFileDialog::getExistingDirectory(this, tr("Specify directory:"));
	mUi->qrealSourcesLineEdit->setText(location);
}

void VisualInterpreterPreferencesPage::save() {
	SettingsManager::setValue("qrealSourcesLocation", mUi->qrealSourcesLineEdit->text());
}

