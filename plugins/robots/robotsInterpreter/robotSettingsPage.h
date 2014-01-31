#pragma once

#include "../../../../qrgui/dialogs/preferencesPages/preferencesPage.h"

#include "sensorConstants.h"
#include "details/sensorsConfigurationWidget.h"
#include "details/sensorsConfigurationProvider.h"

namespace Ui {
	class PreferencesRobotSettingsPage;
}

namespace qReal {
namespace interpreters {
namespace robots {

/// Preferences page for robots interpreter plugin
class PreferencesRobotSettingsPage : public PreferencesPage, public details::SensorsConfigurationProvider
{
	Q_OBJECT

public:
	explicit PreferencesRobotSettingsPage(QWidget *parent = 0);
	~PreferencesRobotSettingsPage();

	void save();
	virtual void restoreSettings();
	void changeTextVisibleOnSettingPage(bool isChecked);

	QString selectedPortName() const;
	QString selectedCommunication() const;

signals:
	void saved();
	void textVisibleChanged(bool isChecked);

protected:
	void changeEvent(QEvent *e);

private slots:
	void onSomethingChanged();
	void manualComPortCheckboxChecked(bool state);

private:
	void initRobotModelType(enums::robotModelType::robotModelTypeEnum type);
	void initTypeOfCommunication(QString const &type);

	enums::robotModelType::robotModelTypeEnum selectedRobotModel() const;
	void refreshValuesOnUi();
	bool textVisible() const;
	int sensorUpdateInterval() const;
	int autoscalingInterval() const;
	int textUpdateInterval() const;

	Ui::PreferencesRobotSettingsPage *mUi;
	details::SensorsConfigurationWidget *mSensorsWidget;
};

}
}
}
