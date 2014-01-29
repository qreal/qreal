#pragma once

#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QRadioButton>

#include <qrgui/dialogs/preferencesPages/preferencesPage.h>

//#include "sensorConstants.h"
//#include "details/sensorsConfigurationWidget.h"
#include "kitPluginManager.h"

namespace Ui {
class PreferencesRobotSettingsPage;
}

namespace interpreterCore {

/// Preferences page for robots interpreter plugin. Contains constructor kit selector and a placeholder for
/// kit-specific settings.
class RobotsSettingsPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit RobotsSettingsPage(KitPluginManager &kitPluginManager, QWidget *parent = 0);
	~RobotsSettingsPage();

	virtual void save();
	virtual void restoreSettings();

signals:
	void saved();

protected:
	void changeEvent(QEvent *e);

private slots:
	void onKitRadioButtonToggled(bool checked);
	void onRobotModelRadioButtonToggled(bool checked);

private:
	void initializeKitRadioButtons();
	QButtonGroup *initializeRobotModelsButtons(QString const &kitId, QRadioButton * const kitButton);
	void showRadioButtonGroup(QWidget * const container
			, QButtonGroup * const radioButtons
			, QWidget * const emptyCaseWidget = nullptr);

	void saveSelectedRobotModel();
	void checkSelectedRobotModelButtonFor(QAbstractButton * const kitButton);

	Ui::PreferencesRobotSettingsPage *mUi;
	KitPluginManager &mKitPluginManager;
	QButtonGroup *mKitButtons;
	QHash<QAbstractButton *, QButtonGroup *> mKitRobotModels;
	QHash<QAbstractButton *, interpreterBase::robotModel::RobotModelInterface *> mButtonsToRobotModelsMapping;
};

}
