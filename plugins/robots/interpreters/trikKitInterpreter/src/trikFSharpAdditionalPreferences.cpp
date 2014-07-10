#include "trikFSharpAdditionalPreferences.h"
#include "ui_trikFSharpAdditionalPreferences.h"

#include <qrkernel/settingsManager.h>

using namespace trikKitInterpreter;
using namespace qReal;

TrikFSharpAdditionalPreferences::TrikFSharpAdditionalPreferences(QWidget *parent)
    : AdditionalPreferences(parent)
    , mUi(new Ui::TrikFSharpAdditionalPreferences)
{
    mUi->setupUi(this);
}

TrikFSharpAdditionalPreferences::~TrikFSharpAdditionalPreferences()
{
    delete mUi;
}

void TrikFSharpAdditionalPreferences::save()
{
    SettingsManager::setValue("FSharpPath", mUi->fSharpLineEdit->text());
    emit settingsChanged();
}

void TrikFSharpAdditionalPreferences::restoreSettings()
{
    mUi->fSharpLineEdit->setText(SettingsManager::value("FSharpPath").toString());
}

