#include "trikWinScpAdditionalPreferences.h"
#include "ui_trikWinScpAdditionalPreferences.h"

#include <qrkernel/settingsManager.h>

using namespace trikKitInterpreter;
using namespace qReal;

TrikWinScpAdditionalPreferences::TrikWinScpAdditionalPreferences(QWidget *parent)
    : AdditionalPreferences(parent)
    , mUi(new Ui::TrikWinScpAdditionalPreferences)
{
    mUi->setupUi(this);
}

TrikWinScpAdditionalPreferences::~TrikWinScpAdditionalPreferences()
{
    delete mUi;
}

void TrikWinScpAdditionalPreferences::save()
{
    SettingsManager::setValue("WinScpPath", mUi->winScpLineEdit->text());
    emit settingsChanged();
}

void TrikWinScpAdditionalPreferences::restoreSettings()
{
    mUi->winScpLineEdit->setText(SettingsManager::value("WinScpPath").toString());
}

