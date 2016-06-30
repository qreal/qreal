/* Copyright 2013-2016 CyberTech Labs Ltd.
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

#include "trikKitInterpreterCommon/trikKitInterpreterPluginBase.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>

#include <twoDModel/engine/twoDModelEngineFacade.h>
#include <qrkernel/settingsManager.h>
#include <qrkernel/settingsListener.h>

using namespace trik;
using namespace qReal;

const Id robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
const Id subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

TrikKitInterpreterPluginBase::TrikKitInterpreterPluginBase()
{
}

TrikKitInterpreterPluginBase::~TrikKitInterpreterPluginBase()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
	}

	if (mOwnsBlocksFactory) {
		delete mBlocksFactory;
	}
}

void TrikKitInterpreterPluginBase::initKitInterpreterPluginBase
		(robotModel::TrikRobotModelBase * const realRobotModel
		, robotModel::twoD::TrikTwoDRobotModel * const twoDRobotModel
		, robotModel::twoD::TrikThreeDRobotModel * const threeDRobotModel
		//, robotModel::twoD::TrikTwoDRobotModel * const threeDRobotModel
		, blocks::TrikBlocksFactoryBase * const blocksFactory
		)
{
	mRealRobotModel.reset(realRobotModel);
	mTwoDRobotModel.reset(twoDRobotModel);
	mThreeDRobotModel.reset(threeDRobotModel);
	mBlocksFactory = blocksFactory;

	const auto modelEngine2D = new twoDModel::engine::TwoDModelEngineFacade(*mTwoDRobotModel, 2);
	const auto modelEngine3D = new twoDModel::engine::TwoDModelEngineFacade(*mThreeDRobotModel, 3);

	mTwoDRobotModel->setEngine(modelEngine2D->engine());
	mThreeDRobotModel->setEngine(modelEngine3D->engine());
	mTwoDModel.reset(modelEngine2D);
	mThreeDModel.reset(modelEngine3D);

	mAdditionalPreferences = new TrikAdditionalPreferences({ mRealRobotModel->name() });
}

void TrikKitInterpreterPluginBase::init(const kitBase::KitPluginConfigurator &configurer)
{
	connect(&configurer.eventsForKitPlugin()
			, &kitBase::EventsForKitPluginInterface::robotModelChanged
			, [this](const QString &modelName) { mCurrentlySelectedModelName = modelName; });

	qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			= configurer.qRealConfigurator().mainWindowInterpretersInterface();

	mTwoDModel->init(configurer.eventsForKitPlugin()
			, configurer.qRealConfigurator().systemEvents()
			, configurer.qRealConfigurator().logicalModelApi()
			, interpretersInterface
			, configurer.qRealConfigurator().projectManager()
			, configurer.interpreterControl());


	mThreeDModel->init(configurer.eventsForKitPlugin()
			, configurer.qRealConfigurator().systemEvents()
			, configurer.qRealConfigurator().logicalModelApi()
			, interpretersInterface
			, configurer.qRealConfigurator().projectManager()
			, configurer.interpreterControl());

	mRealRobotModel->setErrorReporter(*interpretersInterface.errorReporter());
	mTwoDRobotModel->setErrorReporter(*interpretersInterface.errorReporter());

	mThreeDRobotModel->setErrorReporter(*interpretersInterface.errorReporter());

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, mRealRobotModel.data(), &robotModel::TrikRobotModelBase::rereadSettings);

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, mTwoDRobotModel.data(), &robotModel::twoD::TrikTwoDRobotModel::rereadSettings);


	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, mThreeDRobotModel.data(), &robotModel::twoD::TrikTwoDRobotModel::rereadSettings);
}

QList<kitBase::robotModel::RobotModelInterface *> TrikKitInterpreterPluginBase::robotModels()
{
	return {mRealRobotModel.data(), mTwoDRobotModel.data(), mThreeDRobotModel.data()};
//	return {mRealRobotModel.data(), mTwoDRobotModel.data()};
}

kitBase::blocksBase::BlocksFactoryInterface *TrikKitInterpreterPluginBase::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model);

	mOwnsBlocksFactory = false;
	return mBlocksFactory;
}

kitBase::robotModel::RobotModelInterface *TrikKitInterpreterPluginBase::defaultRobotModel()
{
	return mTwoDRobotModel.data();
}

QList<kitBase::AdditionalPreferences *> TrikKitInterpreterPluginBase::settingsWidgets()
{
	mOwnsAdditionalPreferences = false;
	return {mAdditionalPreferences};
}

QWidget *TrikKitInterpreterPluginBase::quickPreferencesFor(const kitBase::robotModel::RobotModelInterface &model)
{
	return model.name().toLower().contains("twod")
			? nullptr
			: produceIpAddressConfigurer();
}

QList<qReal::ActionInfo> TrikKitInterpreterPluginBase::customActions()
{
	return {};
}

QList<HotKeyActionInfo> TrikKitInterpreterPluginBase::hotKeyActions()
{
	return {};
}

QString TrikKitInterpreterPluginBase::defaultSettingsFile() const
{
	return ":/trikDefaultSettings.ini";
}

QIcon TrikKitInterpreterPluginBase::iconForFastSelector(
		const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	return &robotModel == mRealRobotModel.data()
			? QIcon(":/icons/switch-real-trik.svg")
			: &robotModel == mTwoDRobotModel.data()
					? QIcon(":/icons/switch-2d.svg")
					: &robotModel == mThreeDRobotModel.data()
							? QIcon(":/icons/switch-3d.svg")
							: QIcon();
}

kitBase::DevicesConfigurationProvider *TrikKitInterpreterPluginBase::devicesConfigurationProvider()
{
	return &mTwoDModel->devicesConfigurationProvider();
}

QWidget *TrikKitInterpreterPluginBase::produceIpAddressConfigurer()
{
	QLineEdit * const quickPreferences = new QLineEdit;
	quickPreferences->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	quickPreferences->setPlaceholderText(tr("Enter robot`s IP-address here..."));
	const auto updateQuickPreferences = [quickPreferences]() {
		const QString ip = qReal::SettingsManager::value("TrikTcpServer").toString();
		if (quickPreferences->text() != ip) {
			quickPreferences->setText(ip);
		}
	};

	updateQuickPreferences();
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged, updateQuickPreferences);
	qReal::SettingsListener::listen("TrikTcpServer", updateQuickPreferences, this);
	connect(quickPreferences, &QLineEdit::textChanged, [](const QString &text) {
		qReal::SettingsManager::setValue("TrikTcpServer", text);
	});

	connect(this, &QObject::destroyed, [quickPreferences]() { delete quickPreferences; });
	return quickPreferences;
}
