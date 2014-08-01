#pragma once

#include <interpreterBase/additionalPreferences.h>

namespace Ui {
class TrikWinScpAdditionalPreferences;
}

namespace trikKitInterpreter {

class TrikWinScpAdditionalPreferences : public interpreterBase::AdditionalPreferences
{
    Q_OBJECT

public:
    explicit TrikWinScpAdditionalPreferences(QWidget *parent = 0);
    ~TrikWinScpAdditionalPreferences();

    void save() override;
    void restoreSettings() override;

signals:
    void settingsChanged();

private:
    Ui::TrikWinScpAdditionalPreferences *mUi;
};

}
