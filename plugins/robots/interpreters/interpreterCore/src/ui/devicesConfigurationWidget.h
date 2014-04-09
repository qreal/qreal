#pragma once

#include <QtCore/QMap>
#include <QtWidgets/QWidget>

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

/// A number of combo boxes for devices selection
class DevicesConfigurationWidget : public QWidget, public interpreterBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	/// @param autosaveMode Should save happed on each modification or would
	/// be called manually
	explicit DevicesConfigurationWidget(QWidget *parent = 0, bool autosaveMode = false);

	void loadRobotModels(QList<interpreterBase::robotModel::RobotModelInterface *> const &models);

	/// Reinits current widget for the given robot model
	void selectRobotModel(interpreterBase::robotModel::RobotModelInterface &robotModel);

public slots:
	void refresh();
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
	QVBoxLayout *mLayout;
	QString mCurrentModel;
	QMap<QString, interpreterBase::robotModel::RobotModelInterface *> mRobotModels;  // Does not take ownership
	QMap<QString, QWidget *> mRobotModelConfigurers;  // Does not take ownership
	QList<QComboBox *> mConfigurers;
	bool mSaving;
	bool mRefreshing;
};

}
}
