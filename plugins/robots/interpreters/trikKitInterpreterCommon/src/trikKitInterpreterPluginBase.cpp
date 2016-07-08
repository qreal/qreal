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
	: mTakeSnapshotAction(new QAction(nullptr))
	, mSnapshotWindow(new SnapshotWindow(QObject::tr("Snapshot")))
{
	mTakeSnapshotAction->setText(tr("Take snapshot"));
	mTakeSnapshotAction->setCheckable(true);

	connect(mTakeSnapshotAction.data()
			, &QAction::triggered
			, this
			, &TrikKitInterpreterPluginBase::onTakeSnapshotButtonClicked);
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
		, blocks::TrikBlocksFactoryBase * const blocksFactory
		)
{
	mRealRobotModel.reset(realRobotModel);
	mTwoDRobotModel.reset(twoDRobotModel);
	mBlocksFactory = blocksFactory;

	connect(mRealRobotModel.data()
			, &robotModel::TrikRobotModelBase::snapshotReceived
			, this
			, &TrikKitInterpreterPluginBase::snapshotReceived);

	const auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(*mTwoDRobotModel);

	mTwoDRobotModel->setEngine(modelEngine->engine());
	mTwoDModel.reset(modelEngine);

	mAdditionalPreferences = new TrikAdditionalPreferences({ mRealRobotModel->name() });
}

void TrikKitInterpreterPluginBase::init(const kitBase::KitPluginConfigurator &configurer)
{
	connect(mRealRobotModel.data()
			, &kitBase::robotModel::RobotModelInterface::disconnected
			, [this]() { mTakeSnapshotAction->setChecked(false); mReceivingSnapshots = false; });

	qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			= configurer.qRealConfigurator().mainWindowInterpretersInterface();

	mTwoDModel->init(configurer.eventsForKitPlugin()
			, configurer.qRealConfigurator().systemEvents()
			, configurer.qRealConfigurator().logicalModelApi()
			, interpretersInterface
			, configurer.qRealConfigurator().projectManager()
			, configurer.interpreterControl());

	mRealRobotModel->setErrorReporter(*interpretersInterface.errorReporter());
	mTwoDRobotModel->setErrorReporter(*interpretersInterface.errorReporter());

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, mRealRobotModel.data(), &robotModel::TrikRobotModelBase::rereadSettings);

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, mTwoDRobotModel.data(), &robotModel::twoD::TrikTwoDRobotModel::rereadSettings);

	connect(&configurer.eventsForKitPlugin()
			, &kitBase::EventsForKitPluginInterface::robotModelChanged
			, this
			, &TrikKitInterpreterPluginBase::onCurrentRobotModelChanged);

	configurer.qRealConfigurator().mainWindowDockInterface()
			.addDockWidget(Qt::RightDockWidgetArea, mSnapshotWindow.data());

	mSnapshotWindow->setVisible(true);
}

void TrikKitInterpreterPluginBase::onCurrentRobotModelChanged(const QString &modelName)
{
	const bool isThisModel = modelName == mRealRobotModel->name();
	mTakeSnapshotAction->setVisible(isThisModel);

	if (!isThisModel && mReceivingSnapshots) {
		stopTakingSnapshots();
	}

	mCurrentlySelectedModelName = modelName;
}

QList<kitBase::robotModel::RobotModelInterface *> TrikKitInterpreterPluginBase::robotModels()
{
	return {mRealRobotModel.data(), mTwoDRobotModel.data()};
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
	const ActionInfo takeSnapshotActionInfo(mTakeSnapshotAction.data(), "interpreters", "tools");
	return {takeSnapshotActionInfo};
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

void TrikKitInterpreterPluginBase::onTakeSnapshotButtonClicked()
{
	if (mRealRobotModel->connectionState()
			== kitBase::robotModel::RobotModelInterface::ConnectionState::disconnectedState)
	{
		mTakeSnapshotAction->setChecked(false);
		return;
	}

	mReceivingSnapshots ? stopTakingSnapshots() : takeSnapshot();
}

void TrikKitInterpreterPluginBase::takeSnapshot()
{
	mRealRobotModel->takeSnapshot();
	mReceivingSnapshots = true;
	mTakeSnapshotAction->setChecked(true);
}

void TrikKitInterpreterPluginBase::stopTakingSnapshots()
{
	mRealRobotModel->stopTakingSnapshots();
	mReceivingSnapshots = false;
	mTakeSnapshotAction->setChecked(false);
}

void TrikKitInterpreterPluginBase::snapshotReceived(QByteArray snapshot)
{
	mSnapshotWindow.data()->setImage(snapshot);
}
