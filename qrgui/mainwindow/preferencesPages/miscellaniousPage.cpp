#include <QFileDialog>
#include <QSettings>

#include "preferencesPages/miscellaniousPage.h"
#include "ui_miscellaniousPage.h"

PreferencesMiscellaniousPage::PreferencesMiscellaniousPage(QWidget *parent) :
	PreferencesPage(parent),
	ui(new Ui::preferencesMiscellaniousPage)
{
	ui->setupUi(this);

	connect(ui->imagesPathBrowseButton, SIGNAL(clicked()), this, SLOT(browseImagesPath()));

	QSettings settings("SPbSU", "QReal");
	ui->chaoticEditionCheckBox->setChecked(settings.value("ChaoticEdition", false).toBool());
	ui->antialiasingCheckBox->setChecked(settings.value("Antialiasing", true).toBool());
	ui->splashScreenCheckBox->setChecked(settings.value("Splashscreen", true).toBool());
	ui->openGLCheckBox->setChecked(settings.value("OpenGL", true).toBool());
	ui->squareLineModeCheckBox->setChecked(settings.value("SquareLine", false).toBool());

	mLastIconsetPath = settings.value("pathToImages", QDir::currentPath() + "/images/iconset1").toString();
	ui->imagesPathEdit->setText(mLastIconsetPath);
}

PreferencesMiscellaniousPage::~PreferencesMiscellaniousPage()
{
	delete ui;
}

void PreferencesMiscellaniousPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesMiscellaniousPage::browseImagesPath()
{
	QString path = QFileDialog::getExistingDirectory(this, "Open Directory");
	ui->imagesPathEdit->setText(path.replace("\\", "/"));
}

void PreferencesMiscellaniousPage::save()
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("Splashscreen", ui->splashScreenCheckBox->isChecked());
	settings.setValue("Antialiasing", ui->antialiasingCheckBox->isChecked());
	settings.setValue("OpenGL", ui->openGLCheckBox->isChecked());
	settings.setValue("SquareLine", ui->squareLineModeCheckBox->isChecked());
	settings.setValue("ChaoticEdition", ui->chaoticEditionCheckBox->isChecked());
	settings.setValue("pathToImages", ui->imagesPathEdit->text());

	if (mLastIconsetPath != ui->imagesPathEdit->text())
		emit iconsetChanged();
}
