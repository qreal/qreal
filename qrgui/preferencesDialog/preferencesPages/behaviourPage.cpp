#include "behaviourPage.h"
#include "ui_behaviourPage.h"

#include <QtCore/QDir>

#include <qrkernel/settingsManager.h>

using namespace qReal;

PreferencesBehaviourPage::PreferencesBehaviourPage(QWidget *parent)
	: PreferencesPage(parent)
	, mUi(new Ui::PreferencesBehaviourPage)
{
	mUi->setupUi(this);
	setObjectName("preferencesBehaviourPage");
	setWindowIcon(QIcon(":/preferencesDialog/images/behaviour.png"));

	initLanguages();

	connect(mUi->autoSaveCheckBox, SIGNAL(clicked(bool)), this, SLOT(showAutoSaveBox(bool)));
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
	const QString language = mUi->languageComboBox->itemData(mUi->languageComboBox->currentIndex()).toString();
	SettingsManager::setValue("systemLocale", language);
	if (mOldLanguage != language) {
		setRestartFlag();
	}

	SettingsManager::setValue("PaletteTabSwitching", mUi->paletteTabCheckBox->isChecked());
	SettingsManager::setValue("Autosave", mUi->autoSaveCheckBox->isChecked());
	SettingsManager::setValue("AutosaveInterval", mUi->autoSaveSpinBox->value());
	SettingsManager::setValue("gestureDelay", mUi->gestureDelaySpinBox->value());
	SettingsManager::setValue("touchMode", mUi->touchModeCheckBox->isChecked());
}

void PreferencesBehaviourPage::restoreSettings()
{
	const QString locale = SettingsManager::value("systemLocale").toString();
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
	mUi->touchModeCheckBox->setChecked(SettingsManager::value("touchMode").toBool());

	showAutoSaveBox(mUi->autoSaveCheckBox->isChecked());
	const int editorsLoadedCount = SettingsManager::value("EditorsLoadedCount").toInt();
	mUi->paletteTabCheckBox->setVisible(editorsLoadedCount != 1);
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
	for (const QString &locale: translationsDir.entryList(QDir::Dirs)) {
		const QString language = QLocale(locale).nativeLanguageName();
		if (!language.isEmpty()) {
			const QString capitalizedLanguage = language[0].toUpper() + language.mid(1);
			mUi->languageComboBox->addItem(capitalizedLanguage, locale);
		}
	}
}
