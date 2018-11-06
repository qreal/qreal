/* Copyright 2014-2018 CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "trikKitInterpreterCommon/trikAdditionalPreferences.h"
#include "ui_trikAdditionalPreferences.h"

#include <qrkernel/settingsManager.h>
#include <qrutils/widgets/qRealFileDialog.h>

using namespace trik;
using namespace qReal;

TrikAdditionalPreferences::TrikAdditionalPreferences(const QStringList &realRobotNames, QWidget *parent)
	: AdditionalPreferences(parent)
	, mUi(new Ui::TrikAdditionalPreferences)
	, mRealRobotNames(realRobotNames)
{
	mUi->setupUi(this);
	mUi->robotImagePicker->configure("trikRobot2DImage", tr("2D robot image:"));

	mUi->simulatedCameraFrame->setEnabled(not mUi->realCameraCheckBox->isChecked());

	connect(mUi->realCameraCheckBox, &QCheckBox::clicked, [this](bool checked) {
		mUi->simulatedCameraFrame->setEnabled(not checked);
	});

	connect(mUi->browseImagesPathButton, &QPushButton::clicked, [this]() {
		const QString directoryName = utils::QRealFileDialog::getExistingDirectory("TrikSimulatedCameraImagesPath"
					, this, tr("Select Directory")).replace("\\", "/");
		mUi->imagesPathlineEdit->setText(directoryName);
	});
}

TrikAdditionalPreferences::~TrikAdditionalPreferences()
{
	delete mUi;
}

void TrikAdditionalPreferences::save()
{
	SettingsManager::setValue("TrikTcpServer", mUi->tcpServerLineEdit->text());
	SettingsManager::setValue("TrikWebCameraReal", mUi->realCameraCheckBox->isChecked());
	SettingsManager::setValue("TrikSimulatedCameraImagesPackToProject", mUi->packImagesCheckBox->isChecked());
	SettingsManager::setValue("TrikSimulatedCameraImagesPath", mUi->imagesPathlineEdit->text());
	mUi->robotImagePicker->save();
	emit settingsChanged();
}

void TrikAdditionalPreferences::restoreSettings()
{
	mUi->tcpServerLineEdit->setText(SettingsManager::value("TrikTcpServer").toString());
	mUi->packImagesCheckBox->setChecked(SettingsManager::value("TrikSimulatedCameraImagesPackToProject").toBool());
	mUi->realCameraCheckBox->setChecked(SettingsManager::value("TrikWebCameraReal").toBool());
	mUi->imagesPathlineEdit->setText(SettingsManager::value("TrikSimulatedCameraImagesPath").toString());
	mUi->simulatedCameraFrame->setEnabled(not mUi->realCameraCheckBox->isChecked());
	mUi->robotImagePicker->restore();
}

void TrikAdditionalPreferences::onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel)
{
	const bool isReal = !robotModel->name().contains("TwoD");
	mUi->tcpSettingsGroupBox->setVisible(isReal);
	mUi->multimediaSettingsGroupBox->setVisible(not isReal);
}
