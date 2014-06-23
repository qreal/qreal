#pragma once

#include <QtCore/QMap>
#include <QtWidgets/QScrollArea>

#include <interpreterBase/devicesConfigurationProvider.h>
#include <interpreterBase/robotModel/portInfo.h>
#include <interpreterBase/robotModel/deviceInfo.h>

class QComboBox;
class QVBoxLayout;

namespace interpreterBase {
namespace robotModel {
class RobotModelInterface;
}
}

namespace interpreterCore {
namespace ui {

/// A number of combo boxes for devices selection.
class DevicesConfigurationWidget : public QScrollArea, public interpreterBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param autosaveMode - should save happen on each modification or would be called manually.
	explicit DevicesConfigurationWidget(QWidget *parent = 0, bool autosaveMode = false);

	/// Creates configuration widgets for all robot models in a list and prepares them to be shown when
	/// selectRobotModel() is called.
	void loadRobotModels(QList<interpreterBase::robotModel::RobotModelInterface *> const &models);

	/// Reinits current widget for the given robot model.
	void selectRobotModel(interpreterBase::robotModel::RobotModelInterface &robotModel);

public slots:
	/// Updates widget from current model configuration.
	void refresh();

	/// Updates configuration of robot model using current widget settings (via DevicesConfigurationProvider mechanism).
	void save();

private:
	QWidget *configurerForRobotModel(interpreterBase::robotModel::RobotModelInterface &robotModel);

	QLayout *initPort(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, QList<interpreterBase::robotModel::DeviceInfo> const &sensors);

	void hideAllConfigurers();

	void onDeviceConfigurationChanged(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &sensor);

	void propagateChanges(interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &sensor);

	bool areConvertible(interpreterBase::robotModel::PortInfo const &port1
			, interpreterBase::robotModel::PortInfo const &port2) const;
	interpreterBase::robotModel::DeviceInfo convertibleDevice(
			interpreterBase::robotModel::RobotModelInterface const *robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &device) const;

	bool mAutosaveMode;
	QString mCurrentModel;
	QMap<QString, interpreterBase::robotModel::RobotModelInterface *> mRobotModels;  // Does not have ownership.
	QMap<QString, QWidget *> mRobotModelConfigurers;  // Does not have ownership.
	QList<QComboBox *> mConfigurers;  // Has indirect ownership of comboboxes via Qt parent-child memory management.
	bool mSaving;
	bool mRefreshing;
};

}
}
