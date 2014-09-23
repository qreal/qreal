#include "behaviourPage.h"
#include "ui_behaviourPage.h"

#include <QtCore/QDir>

#include <qrkernel/settingsManager.h>
#include <qrutils/uxInfo/uxInfo.h>

using namespace qReal;

PreferencesBehaviourPage::PreferencesBehaviourPage(QWidget *parent)
	: PreferencesPage(parent)
	, mUi(new Ui::PreferencesBehaviourPage)
{
	mUi->setupUi(this);
	setObjectName("preferencesBehaviourPage");
	setWindowIcon(QIcon(":/icons/preferences/behaviour.png"));

	initLanguages();

	connect(mUi->autoSaveCheckBox, SIGNAL(clicked(bool)), this, SLOT(showAutoSaveBox(bool)));
	/// @todo: Get rid of notifying components directly
	connect(mUi->collectErgonomicValuesCheckBox, &QAbstractButton::clicked
			, [](bool status) { utils::UXInfo::setStatus(status); });
	restoreSettings();
}

PreferencesBehaviourPage::~PreferencesBehaviourPage()
{
	delete mUi;
}

void PreferencesBehaviourPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesBehaviourPage::save()
{
	QString const language = mUi->languageComboBox->itemData(mUi->languageComboBox->currentIndex()).toString();
	SettingsManager::setValue("systemLocale", language);
	if (mOldLanguage != language) {
		setRestartFlag();
	}

	SettingsManager::setValue("PaletteTabSwitching", mUi->paletteTabCheckBox->isChecked());
	SettingsManager::setValue("Autosave", mUi->autoSaveCheckBox->isChecked());
	SettingsManager::setValue("AutosaveInterval", mUi->autoSaveSpinBox->value());
	SettingsManager::setValue("gestureDelay", mUi->gestureDelaySpinBox->value());
	SettingsManager::setValue("updaterActive", mUi->checkForUpdatesCheckBox->isChecked());
	bool const usabilityTestingMode = mUi->usabilityModeCheckBox->isChecked();
	SettingsManager::setValue("usabilityTestingMode", usabilityTestingMode);
	SettingsManager::setValue("collectErgonomicValues", mUi->collectErgonomicValuesCheckBox->isChecked()
			|| usabilityTestingMode);
	SettingsManager::setValue("touchMode", mUi->touchModeCheckBox->isChecked());
	if (mUsabilityTestingMode != usabilityTestingMode) {
		if (usabilityTestingMode) {
			mUi->collectErgonomicValuesCheckBox->setChecked(true);
		}

		mUsabilityTestingMode = usabilityTestingMode;
	}
}

void PreferencesBehaviourPage::restoreSettings()
{
	QString const locale = SettingsManager::value("systemLocale").toString();
	mOldLanguage = locale;
	for (int index = 0; index < mUi->languageComboBox->count(); ++index) {
		if (locale == mUi->languageComboBox->itemData(index).toString()) {
			mUi->languageComboBox->setCurrentIndex(index);
		}
	}

	mUi->paletteTabCheckBox->setChecked(SettingsManager::value("PaletteTabSwitching").toBool());
	mUi->autoSaveCheckBox->setChecked(SettingsManager::value("Autosave").toBool());
	mUi->autoSaveSpinBox->setValue(SettingsManager::value("AutosaveInterval").toInt());
	mUi->gestureDelaySpinBox->setValue(SettingsManager::value("gestureDelay").toInt());
	mUi->checkForUpdatesCheckBox->setChecked(SettingsManager::value("updaterActive").toBool());
	mUi->collectErgonomicValuesCheckBox->setChecked(SettingsManager::value("collectErgonomicValues").toBool());
	mUsabilityTestingMode = SettingsManager::value("usabilityTestingMode").toBool();
	mUi->usabilityModeCheckBox->setChecked(mUsabilityTestingMode);
	mUi->touchModeCheckBox->setChecked(SettingsManager::value("touchMode").toBool());

	showAutoSaveBox(mUi->autoSaveCheckBox->isChecked());
	int const editorsLoadedCount = SettingsManager::value("EditorsLoadedCount").toInt();
	mUi->paletteTabCheckBox->setVisible(editorsLoadedCount != 1);
	/// @todo: Get rid of notifying components directly
	utils::UXInfo::setStatus(mUi->collectErgonomicValuesCheckBox->isChecked());
}

void PreferencesBehaviourPage::showAutoSaveBox(bool show)
{
	mUi->autoSaveSpinBox->setVisible(show);
	mUi->autoSaveLabel->setVisible(show);
}

void PreferencesBehaviourPage::initLanguages()
{
	mUi->languageComboBox->addItem(tr("<System Language>"));
	mUi->languageComboBox->addItem("English", "en");
	QDir translationsDir(QApplication::applicationDirPath() + "/translations");
	for (QString const &locale: translationsDir.entryList(QDir::Dirs)) {
		QString const language = QLocale(locale).nativeLanguageName();
		if (!language.isEmpty()) {
			QString const capitalizedLanguage = language[0].toUpper() + language.mid(1);
			mUi->languageComboBox->addItem(capitalizedLanguage, locale);
		}
	}
}
