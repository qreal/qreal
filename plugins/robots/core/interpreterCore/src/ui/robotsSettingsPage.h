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

#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QRadioButton>

#include <qrgui/preferencesDialog/preferencesPage.h>
#include <kitBase/devicesConfigurationProvider.h>

#include "interpreterCore/managers/kitPluginManager.h"
#include "interpreterCore/managers/robotModelManager.h"

namespace Ui {
class PreferencesRobotSettingsPage;
}

namespace interpreterCore {
namespace ui {

/// Preferences page for robots interpreter plugin. Contains constructor kit selector and a placeholder for
/// kit-specific settings.
class RobotsSettingsPage : public qReal::gui::PreferencesPage, public kitBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param kitPluginManager - reference to kit plugin manager.
	/// @param robotModelManager - contains information about currently selected robot model.
	/// @param parent - parent of this window in terms of Qt widget system.
	explicit RobotsSettingsPage(
			KitPluginManager &kitPluginManager
			, RobotModelManager &robotModelManager
			, qReal::LogicalModelAssistInterface &logicalModel
			, QWidget *parent = nullptr
			);

	~RobotsSettingsPage() override;

	void save() override;
	void restoreSettings() override;

public slots:
	/// Called when current project changed, allows settings page to react on save-specific settings.
	void onProjectOpened();

signals:
	/// Emitted when uder saves settings on this page.
	void saved();

protected:
	void changeEvent(QEvent *e) override;

private slots:
	void onKitRadioButtonToggled(bool checked);
	void onRobotModelRadioButtonToggled(bool checked);

private:
	void initializeAdditionalWidgets();
	void initializeKitRadioButtons();
	QButtonGroup *initializeRobotModelsButtons(const QString &kitId, QRadioButton * const kitButton);

	void showAdditionalPreferences(const QString &kitId);
	void showRadioButtonGroup(QWidget * const container
			, QButtonGroup * const radioButtons
			, QWidget * const emptyCaseWidget = nullptr);

	void saveSelectedRobotModel();
	void checkSelectedRobotModelButtonFor(QAbstractButton * const kitButton);
	void changeRobotModel(QAbstractButton * const kitButton);

	Ui::PreferencesRobotSettingsPage *mUi;  // Has ownership.
	KitPluginManager &mKitPluginManager;
	RobotModelManager &mRobotModelManager;

	/// Has ownership indirectly, via Qt parent-child memory management system.
	QButtonGroup *mKitButtons;

	/// Has ownership indirectly, via Qt parent-child memory management system.
	QHash<QAbstractButton *, QButtonGroup *> mKitRobotModels;

	/// Has ownership over buttons indirectly, via Qt parent-child memory management system.
	/// Does not have ownership over robot models.
	QHash<QAbstractButton *, kitBase::robotModel::RobotModelInterface *> mButtonsToRobotModelsMapping;

	/// Reference to logical model, to be able to change settings for current save, for example, disable sensors
	/// changes when save explicitly prohibits it.
	qReal::LogicalModelAssistInterface &mLogicalModel;
};

}
}
