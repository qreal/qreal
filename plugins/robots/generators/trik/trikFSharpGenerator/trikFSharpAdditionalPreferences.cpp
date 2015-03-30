#include "trikFSharpAdditionalPreferences.h"
#include "ui_trikFSharpAdditionalPreferences.h"

#include <qrkernel/settingsManager.h>

using namespace trik;
using namespace qReal;

TrikFSharpAdditionalPreferences::TrikFSharpAdditionalPreferences(const QString &robotName, QWidget *parent)
	: AdditionalPreferences(parent)
	, mRobotName(robotName)
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
}

void TrikFSharpAdditionalPreferences::restoreSettings()
{
	mUi->fSharpLineEdit->setText(SettingsManager::value("FSharpPath").toString());
}

void TrikFSharpAdditionalPreferences::onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel)
{
	mUi->fSharpSettingsGroupBox->setVisible(robotModel->name() == mRobotName);
}
