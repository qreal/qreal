#pragma once

#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QRadioButton>

#include <qrgui/dialogs/preferencesPages/preferencesPage.h>
#include <interpreterBase/devicesConfigurationProvider.h>

#include "managers/kitPluginManager.h"
#include "managers/robotModelManager.h"

namespace Ui {
class PreferencesRobotSettingsPage;
}

namespace interpreterCore {
namespace ui {

/// Preferences page for robots interpreter plugin. Contains constructor kit selector and a placeholder for
/// kit-specific settings.
class RobotsSettingsPage : public PreferencesPage, public interpreterBase::DevicesConfigurationProvider
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
			, QWidget *parent = nullptr
			);

	~RobotsSettingsPage() override;

	void save() override;
	void restoreSettings() override;

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
	QButtonGroup *initializeRobotModelsButtons(QString const &kitId, QRadioButton * const kitButton);

	void showAdditionalPreferences(QString const &kitId);
	void showRadioButtonGroup(QWidget * const container
			, QButtonGroup * const radioButtons
			, QWidget * const emptyCaseWidget = nullptr);

	void saveSelectedRobotModel();
	void checkSelectedRobotModelButtonFor(QAbstractButton * const kitButton);
	void changeRobotModel(QAbstractButton * const kitButton, bool const isManuallyToggled);

	Ui::PreferencesRobotSettingsPage *mUi;  // Has ownership.
	KitPluginManager &mKitPluginManager;
	RobotModelManager &mRobotModelManager;
	QButtonGroup *mKitButtons;  // Has ownership indirectly, via Qt parent-child memory management system.

	// Has ownership indirectly, via Qt parent-child memory management system.
	QHash<QAbstractButton *, QButtonGroup *> mKitRobotModels;

	// Has ownership over buttons indirectly, via Qt parent-child memory management system.
	// Does not have ownership over robot models.
	QHash<QAbstractButton *, interpreterBase::robotModel::RobotModelInterface *> mButtonsToRobotModelsMapping;
};

}
}
