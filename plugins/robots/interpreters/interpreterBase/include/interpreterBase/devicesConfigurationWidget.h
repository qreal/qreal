#pragma once

/// @todo: More apropriate place for it?

#include <QtCore/QMap>
#include <QtWidgets/QScrollArea>

#include "interpreterBase/devicesConfigurationProvider.h"
#include "interpreterBase/robotModel/portInfo.h"
#include "interpreterBase/robotModel/deviceInfo.h"

class QComboBox;
class QVBoxLayout;

namespace interpreterBase {

namespace robotModel {
class RobotModelInterface;
}

/// A number of combo boxes for devices selection.
class ROBOTS_INTERPRETER_BASE_EXPORT DevicesConfigurationWidget : public QScrollArea, public DevicesConfigurationProvider
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param autosaveMode - should save happen on each modification or would be called manually.
	/// @param compactMode - widget must minimize its horizontal size.
	explicit DevicesConfigurationWidget(QWidget *parent = 0, bool autosaveMode = false, bool compactMode = false);

	/// Creates configuration widgets for all robot models in a list and prepares them to be shown when
	/// selectRobotModel() is called.
	void loadRobotModels(QList<robotModel::RobotModelInterface *> const &models);

	/// Reinits current widget for the given robot model.
	void selectRobotModel(robotModel::RobotModelInterface &robotModel);

	/// Adds the given widget above the combo boxes. The widget is visible only if the given robot model is selected.
	/// loadRobotModels() method must be called before this one.
	void prependCustomWidget(robotModel::RobotModelInterface &robotModel, QWidget * const widget);

public slots:
	/// Updates widget from current model configuration.
	void refresh();

	/// Updates configuration of robot model using current widget settings (via DevicesConfigurationProvider mechanism).
	void save();

private:
	QWidget *configurerForRobotModel(robotModel::RobotModelInterface &robotModel);

	QLayout *initPort(QString const &robotModel
			, robotModel::PortInfo const &port
			, QList<robotModel::DeviceInfo> const &sensors);

	void hideAllConfigurers();

	void onDeviceConfigurationChanged(QString const &robotModel
			, robotModel::PortInfo const &port
			, robotModel::DeviceInfo const &sensor
			, Reason reason) override;

	void propagateChanges(robotModel::PortInfo const &port
			, robotModel::DeviceInfo const &sensor);

	bool areConvertible(robotModel::PortInfo const &port1
			, robotModel::PortInfo const &port2) const;

	robotModel::DeviceInfo convertibleDevice(
			robotModel::RobotModelInterface const *robotModel
			, robotModel::PortInfo const &port
			, robotModel::DeviceInfo const &device) const;

	bool mAutosaveMode;
	bool mCompactMode;
	QString mCurrentModel;
	QMap<QString, robotModel::RobotModelInterface *> mRobotModels;  // Does not have ownership.
	QMap<QString, QWidget *> mRobotModelConfigurers;  // Has indirect ownership via Qt parent-child memory management.
	QList<QComboBox *> mConfigurers;  // Has indirect ownership of comboboxes via Qt parent-child memory management.
	bool mSaving;
	bool mRefreshing;
};

}
