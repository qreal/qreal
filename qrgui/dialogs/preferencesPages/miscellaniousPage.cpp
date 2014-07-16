#include "miscellaniousPage.h"
#include "ui_miscellaniousPage.h"

#include <qrkernel/settingsManager.h>
#include <qrutils/qRealFileDialog.h>

#include <QtWidgets/QComboBox>
#include "configurationNetworkManager/server.h"
using namespace qReal;

PreferencesMiscellaniousPage::PreferencesMiscellaniousPage(QWidget *parent, bool isServer)
	: PreferencesPage(parent)
	, mUi(new Ui::PreferencesMiscellaniousPage)
{
	mIcon = QIcon(":/icons/preferences/miscellaneous.png");
	mUi->setupUi(this);

	if (isServer) {
		QComboBox *serverIPComboBox = new QComboBox;
		serverIPComboBox->addItems(Server::getIP());
		mUi->serverIPEdit->hide();
		mUi->serverPortEdit->setReadOnly(true);
		mUi->serverLayout->addWidget(serverIPComboBox, 0, 1);
	}

	connect(mUi->imagesPathBrowseButton, SIGNAL(clicked()), this, SLOT(browseImagesPath()));
	connect(mUi->toolbarSizeSlider, &QSlider::valueChanged, this, &PreferencesMiscellaniousPage::toolbarSizeChanged);

	mUi->colorComboBox->addItems(QColor::colorNames());

	restoreSettings();
	connect(mUi->settingsButton, &QAbstractButton::clicked, this, &PreferencesMiscellaniousPage::getSettings);
}

void PreferencesMiscellaniousPage::getSettings()
{
	Client *mClient = new Client();
	delete mClient;
	//mClient->connectToSettingsServer();
}

PreferencesMiscellaniousPage::~PreferencesMiscellaniousPage()
{
	delete mUi;
	//delete mClient;
}

void PreferencesMiscellaniousPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesMiscellaniousPage::browseImagesPath()
{
	QString const path = utils::QRealFileDialog::getExistingDirectory("OpenImagesOnMiscellaniousPage",
			this, tr("Open Directory")).replace("\\", "/");
	if (!path.isEmpty()) {
		mUi->imagesPathEdit->setText(path);
	}
}

void PreferencesMiscellaniousPage::save()
{
	SettingsManager::setValue("ServerIP", mUi->serverIPEdit->text());
	SettingsManager::setValue("ServerPort", mUi->serverPortEdit->text());

	SettingsManager::setValue("Splashscreen", mUi->splashScreenCheckBox->isChecked());
	SettingsManager::setValue("Antialiasing", mUi->antialiasingCheckBox->isChecked());

	SettingsManager::setValue("pathToImages", mUi->imagesPathEdit->text());
	SettingsManager::setValue("recentProjectsLimit", mUi->recentProjectsLimitSpinBox->value());
	SettingsManager::setValue("PaintOldEdgeMode", mUi->paintOldLineCheckBox->isChecked());
	SettingsManager::setValue("oldLineColor", mUi->colorComboBox->currentText());

	SettingsManager::setValue("toolbarSize", mUi->toolbarSizeSlider->value());

	if (mLastIconsetPath != mUi->imagesPathEdit->text()) {
		emit iconsetChanged();
	}
}

void PreferencesMiscellaniousPage::restoreSettings()
{
	mUi->serverIPEdit->setText(SettingsManager::value("ServerIP").toString());
	mUi->serverPortEdit->setText(SettingsManager::value("ServerPort").toString());

	mUi->antialiasingCheckBox->setChecked(SettingsManager::value("Antialiasing").toBool());
	mUi->splashScreenCheckBox->setChecked(SettingsManager::value("Splashscreen").toBool());

	mUi->paintOldLineCheckBox->setChecked(SettingsManager::value("PaintOldEdgeMode").toBool());

	QString curColor = SettingsManager::value("oldLineColor").toString();
	int curColorIndex = mUi->colorComboBox->findText(curColor);
	mUi->colorComboBox->setCurrentIndex(curColorIndex);

	mUi->toolbarSizeSlider->setValue(SettingsManager::value("toolbarSize").toInt());

	mLastIconsetPath = SettingsManager::value("pathToImages").toString();
	mUi->imagesPathEdit->setText(mLastIconsetPath);
}
