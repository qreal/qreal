#include <QtGui/QFileDialog>

#include "../../../qrkernel/settingsManager.h"

#include "visualInterpreterPreferencesPage.h"
#include "ui_visualInterpreterPreferencePage.h"


using namespace qReal;

VisualInterpreterPreferencesPage::VisualInterpreterPreferencesPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::visualInterpreterPreferencesPage)
{
	mIcon = QIcon(":/icons/preferences/bug.png");
	mUi->setupUi(this);

	QString const binFolder = qApp->applicationDirPath();
	QDir binDir(binFolder);
	binDir.cdUp();
	SettingsManager::setValue("qrealSourcesLocation", binDir.absolutePath());

	mUi->qrealSourcesLineEdit->setText(SettingsManager::value("qrealSourcesLocation"
			, binFolder.mid(0, binFolder.lastIndexOf("/"))).toString());
	mUi->pythonPathLineEdit->setText(SettingsManager::value("pythonPath").toString());
	mUi->reactionPathLineEdit->setText(SettingsManager::value("reactionPath"
			, binFolder + "/reaction.py").toString());

	connect(mUi->qrealSourcesPushButton, SIGNAL(clicked()), this, SLOT(setQRealSourcesLocation()));
	connect(mUi->pythonPathPushButton, SIGNAL(clicked()), this, SLOT(setPythonPath()));
	connect(mUi->reactionPathPushButton, SIGNAL(clicked()), this, SLOT(setReactionPath()));
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

void VisualInterpreterPreferencesPage::setReactionPath()
{
	QString const path = QFileDialog::getOpenFileName(this, tr("Specify reaction file:"));
	mUi->reactionPathLineEdit->setText(path);
}

void VisualInterpreterPreferencesPage::save()
{
	SettingsManager::setValue("qrealSourcesLocation", mUi->qrealSourcesLineEdit->text());
	SettingsManager::setValue("pythonPath", mUi->pythonPathLineEdit->text());
	SettingsManager::setValue("reactionPath", mUi->reactionPathLineEdit->text());
}

