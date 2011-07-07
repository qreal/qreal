#include "../../kernel/settingsManager.h"
#include "preferencesPages/compilerPage.h"
#include "ui_compilerPage.h"

PreferencesCompilerPage::PreferencesCompilerPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesCompilerPage)
{
	mUi->setupUi(this);

	connect(mUi->linuxButton, SIGNAL(clicked()), this, SLOT(changeSystem()));
	connect(mUi->windowsButton, SIGNAL(clicked()), this, SLOT(changeSystem()));
	connect(mUi->otherButton, SIGNAL(clicked()), this, SLOT(changeSystem()));

	mUi->windowsButton->setChecked(SettingsManager::instance()->value("windowsButton", false).toBool());
	mUi->linuxButton->setChecked(SettingsManager::instance()->value("linuxButton", false).toBool());
	mUi->otherButton->setChecked(SettingsManager::instance()->value("otherButton", false).toBool());
	mUi->pathToQmake->setText(SettingsManager::instance()->value("pathToQmake", "").toString());
	mUi->pathToMake->setText(SettingsManager::instance()->value("pathToMake", "").toString());
	mUi->pluginExtension->setText(SettingsManager::instance()->value("pluginExtension", "").toString());
	mUi->prefix->setText(SettingsManager::instance()->value("prefix", "").toString());
}

PreferencesCompilerPage::~PreferencesCompilerPage()
{
	delete mUi;
}

void PreferencesCompilerPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesCompilerPage::changeSystem()
{
	if(mUi->linuxButton->isChecked())
		initCompilersSettings("qmake", "make", "so", "lib");
	if(mUi->windowsButton->isChecked())
		initCompilersSettings("qmake", "mingw32-make", "dll", "");
	if(mUi->otherButton->isChecked())
		mUi->compilerSettingsWidget->setEnabled(true);
}

void PreferencesCompilerPage::initCompilersSettings(const QString &pathToQmake,
		const QString &pathToMake, const QString &pluginExtension, const QString &prefix)
{
	mUi->pathToQmake->setText(pathToQmake);
	mUi->pathToMake->setText(pathToMake);
	mUi->pluginExtension->setText(pluginExtension);
	mUi->prefix->setText(prefix);
	mUi->compilerSettingsWidget->setEnabled(false);
}

void PreferencesCompilerPage::save()
{
	SettingsManager::instance()->setValue("windowsButton", mUi->windowsButton->isChecked());
	SettingsManager::instance()->setValue("linuxButton", mUi->linuxButton->isChecked());
	SettingsManager::instance()->setValue("otherButton", mUi->otherButton->isChecked());
	SettingsManager::instance()->setValue("pathToQmake", mUi->pathToQmake->text());
	SettingsManager::instance()->setValue("pathToMake", mUi->pathToMake->text());
	SettingsManager::instance()->setValue("pluginExtension", mUi->pluginExtension->text());
	SettingsManager::instance()->setValue("prefix", mUi->prefix->text());
}
