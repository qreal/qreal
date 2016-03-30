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

#pragma once

#include <QtCore/QScopedPointer>
#include <QtCore/QSharedPointer>

#include <kitBase/kitPluginInterface.h>
#include <twoDModel/robotModel/twoDRobotModel.h>
#include <twoDModel/engine/twoDModelControlInterface.h>

#include <trikKit/blocks/trikBlocksFactoryBase.h>
#include <trikKit/robotModel/trikRobotModelBase.h>

#include "robotModel/twoD/trikTwoDRobotModel.h"
#include "trikAdditionalPreferences.h"

#include <trikKitInterpreterCommon/trikQtsInterpreter.h>

/// @todo: refactor
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>

#include "declSpec.h"

namespace trik {

class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT TrikKitInterpreterPluginBase
		: public QObject, public kitBase::KitPluginInterface, /*for now*/ public kitBase::DevicesConfigurationProvider
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

	TrikQtsInterpreter * qtsInterpreter() const;

signals:
	started();
	stopped(qReal::interpretation::StopReason reason);

private slots:
	QWidget *produceIpAddressConfigurer();  // Transfers ownership

	void testStart(); // QtS
	void testStop();
	void onTabChanged(const qReal::TabInfo &info);

protected:
	/// Takes ownership over all supplied pointers.
	void initKitInterpreterPluginBase(
			robotModel::TrikRobotModelBase * const realRobotModel
			, robotModel::twoD::TrikTwoDRobotModel * const twoDRobotModel
			, blocks::TrikBlocksFactoryBase * const blocksFactory
			);

	qReal::gui::MainWindowInterpretersInterface *mMainWindow;

private:
	QScopedPointer<twoDModel::TwoDModelControlInterface> mTwoDModel;
	QScopedPointer<robotModel::TrikRobotModelBase> mRealRobotModel;
	QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> mTwoDRobotModel;

	QScopedPointer<TrikQtsInterpreter> mQtsInterpreter;

	QAction mStart;
	QAction mStop;

	bool mIsModelSelected = false;

	qReal::SystemEvents *mSystemEvents = nullptr; // Does not have ownership

	/// @todo Use shared pointers instead of this sh~.
	/// Ownership depends on mOwnsBlocksFactory flag.
	blocks::TrikBlocksFactoryBase *mBlocksFactory = nullptr;
	bool mOwnsBlocksFactory = true;

	/// Ownership depends on mOwnsAdditionalPreferences flag.
	TrikAdditionalPreferences *mAdditionalPreferences = nullptr;
	bool mOwnsAdditionalPreferences = true;

	kitBase::InterpreterControlInterface *mInterpreterControl;  // Does not have ownership.
	QString mCurrentlySelectedModelName;
};

}
