#pragma once

#include <QtCore/QMap>
#include <QtWidgets/QWidget>

#include <interpreterBase/sensorsConfigurationProvider.h>
#include <interpreterBase/robotModel/portInfo.h>
#include <interpreterBase/robotModel/pluggableDeviceInfo.h>

class QComboBox;

namespace interpreterBase {
namespace robotModel {
class RobotModelInterface;
}
}

namespace interpreterCore {
namespace ui {

/// A number of combo boxes for sensors selection
class SensorsConfigurationWidget : public QWidget, public interpreterBase::SensorsConfigurationProvider
{
	Q_OBJECT

public:
	/// @param autosaveMode Should save happed on each modification or would
	/// be called manually
	explicit SensorsConfigurationWidget(QWidget *parent = 0, bool autosaveMode = false);

	/// Reinits current widget for the given robot model
	void loadRobotModel(interpreterBase::robotModel::RobotModelInterface * const robotModel);

public slots:
	void refresh();
	void save();

private:
	QLayout *initPort(interpreterBase::robotModel::PortInfo const &port
			, QList<interpreterBase::robotModel::PluggableDeviceInfo> const &sensors);

	QString settingsKey(interpreterBase::robotModel::PortInfo const &port) const;

	bool mAutosaveMode;
	QString mRobotModelId;
	QMap<QComboBox *, interpreterBase::robotModel::PortInfo> mBoxesToPorts;
};

}
}
