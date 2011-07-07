#include "preferencesPages/compilerPage.h"
#include "ui_compilerPage.h"

#include <QSettings>

PreferencesCompilerPage::PreferencesCompilerPage(QWidget *parent) :
	PreferencesPage(parent),
	ui(new Ui::preferencesCompilerPage)
{
	ui->setupUi(this);

	connect(ui->linuxButton, SIGNAL(clicked()), this, SLOT(changeSystem()));
	connect(ui->windowsButton, SIGNAL(clicked()), this, SLOT(changeSystem()));
	connect(ui->otherButton, SIGNAL(clicked()), this, SLOT(changeSystem()));

	QSettings settings("SPbSU", "QReal");
	ui->windowsButton->setChecked(settings.value("windowsButton", false).toBool());
	ui->linuxButton->setChecked(settings.value("linuxButton", false).toBool());
	ui->otherButton->setChecked(settings.value("otherButton", false).toBool());
	ui->pathToQmake->setText(settings.value("pathToQmake", "").toString());
	ui->pathToMake->setText(settings.value("pathToMake", "").toString());
	ui->pluginExtension->setText(settings.value("pluginExtension", "").toString());
	ui->prefix->setText(settings.value("prefix", "").toString());
}

PreferencesCompilerPage::~PreferencesCompilerPage()
{
	delete ui;
}

void PreferencesCompilerPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesCompilerPage::changeSystem()
{
	if(ui->linuxButton->isChecked())
		initCompilersSettings("qmake", "make", "so", "lib");
	if(ui->windowsButton->isChecked())
		initCompilersSettings("qmake", "mingw32-make", "dll", "");
	if(ui->otherButton->isChecked())
		ui->compilerSettingsWidget->setEnabled(true);
}

void PreferencesCompilerPage::initCompilersSettings(const QString &pathToQmake,
		const QString &pathToMake, const QString &pluginExtension, const QString &prefix)
{
	ui->pathToQmake->setText(pathToQmake);
	ui->pathToMake->setText(pathToMake);
	ui->pluginExtension->setText(pluginExtension);
	ui->prefix->setText(prefix);
	ui->compilerSettingsWidget->setEnabled(false);
}

void PreferencesCompilerPage::save()
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("windowsButton", ui->windowsButton->isChecked());
	settings.setValue("linuxButton", ui->linuxButton->isChecked());
	settings.setValue("otherButton", ui->otherButton->isChecked());
	settings.setValue("pathToQmake", ui->pathToQmake->text());
	settings.setValue("pathToMake", ui->pathToMake->text());
	settings.setValue("pluginExtension", ui->pluginExtension->text());
	settings.setValue("prefix", ui->prefix->text());
}
