#include <QtWidgets/QFileDialog>
#include "../../../qrkernel/settingsManager.h"

#include "visualInterpreterPreferencesPage.h"
#include "ui_visualInterpreterPreferencePage.h"


using namespace qReal;

VisualInterpreterPreferencesPage::VisualInterpreterPreferencesPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::visualInterpreterPreferencesPage)
{
	setWindowIcon(QIcon(":/icons/preferences/bug.png"));
	mUi->setupUi(this);

	QString const binFolder = qApp->applicationDirPath();
	QDir binDir(binFolder);
	binDir.cdUp();
	SettingsManager::setValue("qrealSourcesLocation", binDir.absolutePath());

	mUi->qrealSourcesLineEdit->setText(SettingsManager::value("qrealSourcesLocation"
			, binFolder.mid(0, binFolder.lastIndexOf("/"))).toString());
	mUi->pythonPathLineEdit->setText(SettingsManager::value("pythonPath").toString());
	mUi->tempPathLineEdit->setText(SettingsManager::value("tempScriptPath", binFolder + "/temp.py").toString());
	mUi->genTimeoutSpinBox->setValue(SettingsManager::value("generationTimeout").toInt());

	connect(mUi->qrealSourcesPushButton, SIGNAL(clicked()), this, SLOT(setQRealSourcesLocation()));
	connect(mUi->pythonPathPushButton, SIGNAL(clicked()), this, SLOT(setPythonPath()));
	connect(mUi->tempPathPushButton, SIGNAL(clicked()), this, SLOT(setTempScriptPath()));
}

VisualInterpreterPreferencesPage::~VisualInterpreterPreferencesPage()
{
	delete mUi;
}

void VisualInterpreterPreferencesPage::setQRealSourcesLocation()
{
	QString const location = QFileDialog::getExistingDirectory(this, tr("Specify directory:"));
	mUi->qrealSourcesLineEdit->setText(location);
}

void VisualInterpreterPreferencesPage::setPythonPath()
{
	QString const path = QFileDialog::getOpenFileName(this, tr("Specify python path:"));
	mUi->pythonPathLineEdit->setText(path);
}

void VisualInterpreterPreferencesPage::setTempScriptPath()
{
	QString const path = QFileDialog::getOpenFileName(this, tr("Specify temp script file:"));
	mUi->tempPathLineEdit->setText(path);
}

void VisualInterpreterPreferencesPage::save()
{
	SettingsManager::setValue("qrealSourcesLocation", mUi->qrealSourcesLineEdit->text());
	SettingsManager::setValue("pythonPath", mUi->pythonPathLineEdit->text());
	SettingsManager::setValue("tempScriptPath", mUi->tempPathLineEdit->text());
	SettingsManager::setValue("generationTimeout", mUi->genTimeoutSpinBox->value());
}

void VisualInterpreterPreferencesPage::restoreSettings()
{
	mUi->qrealSourcesLineEdit->setText(SettingsManager::value("qrealSourcesLocation").toString());
}

