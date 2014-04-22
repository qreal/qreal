#include "trikAdditionalPreferences.h"
#include "ui_trikAdditionalPreferences.h"

#include <qrkernel/settingsManager.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialenumerator.h>

using namespace trikKitInterpreter;
using namespace qReal;

TrikAdditionalPreferences::TrikAdditionalPreferences(QString const &realRobotName, QWidget *parent)
	: AdditionalPreferences(parent)
	, mUi(new Ui::TrikAdditionalPreferences)
	, mRealRobotName(realRobotName)
{
	mUi->setupUi(this);
}

TrikAdditionalPreferences::~TrikAdditionalPreferences()
{
	delete mUi;
}

void TrikAdditionalPreferences::save()
{
	SettingsManager::setValue("TrikTcpServer", mUi->tcpServerLineEdit->text());
	emit settingsChanged();
}

void TrikAdditionalPreferences::restoreSettings()
{
	mUi->tcpServerLineEdit->setText(SettingsManager::value("TrikTcpServer").toString());
}

void TrikAdditionalPreferences::onRobotModelChanged(interpreterBase::robotModel::RobotModelInterface * const robotModel)
{
	bool const isReal = robotModel->name() == mRealRobotName;
	mUi->tcpSettingsGroupBox->setVisible(isReal);
}
