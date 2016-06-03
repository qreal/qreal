/* Copyright 2007-2015 QReal Research Group
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

#pragma once

#include <QtCore/QScopedPointer>

#include <kitBase/kitPluginInterface.h>
#include <twoDModel/robotModel/twoDRobotModel.h>
#include <twoDModel/engine/twoDModelControlInterface.h>

#include <trikKit/blocks/trikBlocksFactoryBase.h>
#include <trikKit/robotModel/trikRobotModelBase.h>

#include "robotModel/twoD/trikTwoDRobotModel.h"
#include "trikAdditionalPreferences.h"

#include "declSpec.h"
#include "snapshotWindow.h"

namespace trik {

class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT TrikKitInterpreterPluginBase
		: public QObject, public kitBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)

public:
	TrikKitInterpreterPluginBase();
	~TrikKitInterpreterPluginBase() override;

	void init(const kitBase::KitPluginConfigurator &configurer) override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;

	kitBase::robotModel::RobotModelInterface *defaultRobotModel() override;

	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

	QWidget *quickPreferencesFor(const kitBase::robotModel::RobotModelInterface &model) override;

	QString defaultSettingsFile() const override;

	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;

	kitBase::DevicesConfigurationProvider * devicesConfigurationProvider() override;

	QList<qReal::ActionInfo> customActions() override;

	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

private slots:
	QWidget *produceIpAddressConfigurer();  // Transfers ownership

	void onTakeSnapshotButtonClicked();

	/// Emitted when received a snapshot from robot.
	void snapshotReceived(QByteArray snapshot);

protected:
	/// Takes ownership over all supplied pointers.
	void initKitInterpreterPluginBase(
			robotModel::TrikRobotModelBase * const realRobotModel
			, robotModel::twoD::TrikTwoDRobotModel * const twoDRobotModel
			, blocks::TrikBlocksFactoryBase * const blocksFactory
			);

	void onCurrentRobotModelChanged(const QString &modelName);

private:
	/// Requests real robot to send a snapshot.
	void takeSnapshot();

	/// Requests real robot to stop sending snapshots.
	void stopTakingSnapshots();

	QScopedPointer<twoDModel::TwoDModelControlInterface> mTwoDModel;
	QScopedPointer<robotModel::TrikRobotModelBase> mRealRobotModel;
	QScopedPointer<robotModel::twoD::TrikTwoDRobotModel> mTwoDRobotModel;

	/// @todo Use shared pointers instead of this sh~.
	/// Ownership depends on mOwnsBlocksFactory flag.
	blocks::TrikBlocksFactoryBase *mBlocksFactory = nullptr;
	bool mOwnsBlocksFactory = true;

	/// Ownership depends on mOwnsAdditionalPreferences flag.
	TrikAdditionalPreferences *mAdditionalPreferences = nullptr;
	bool mOwnsAdditionalPreferences = true;

	/// It is true if TRIK Studio is receiving snapshots from real robot now.
	bool mReceivingSnapshots = false;

	kitBase::InterpreterControlInterface *mInterpreterControl; // Does not have ownership.
	QString mCurrentlySelectedModelName;

	/// Action that requests real robot to start sending snapshots.
	QScopedPointer<QAction> mTakeSnapshotAction; // Has ownership

	/// Window that displays snapshot received from real robot.
	QScopedPointer<SnapshotWindow> mSnapshotWindow; // Has ownership
};

}
