#pragma once

#include <interpreterBase/additionalPreferences.h>

namespace Ui {
class TrikFSharpAdditionalPreferences;
}

namespace trikKitInterpreter {

class TrikFSharpAdditionalPreferences : public interpreterBase::AdditionalPreferences
{
    Q_OBJECT

public:
    explicit TrikFSharpAdditionalPreferences(QWidget *parent = 0);
    ~TrikFSharpAdditionalPreferences();

    void save() override;
    void restoreSettings() override;

signals:
    void settingsChanged();

private:
    Ui::TrikFSharpAdditionalPreferences *mUi;
};

}
