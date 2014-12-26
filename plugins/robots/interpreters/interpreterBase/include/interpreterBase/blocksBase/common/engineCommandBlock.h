#pragma once

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>
#include <interpreterBase/robotModel/robotModelUtils.h>
#include <interpreterBase/robotModel/robotParts/motor.h>

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// A base for all blocks that work with motors.
class ROBOTS_INTERPRETER_BASE_EXPORT EngineCommandBlock : public Block
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit EngineCommandBlock(robotModel::RobotModelInterface &robotModel);

	QMap<robotModel::PortInfo, robotModel::DeviceInfo> usedDevices() override;

protected slots:
	/// @todo Why it is needed and where it is used?
	void timeout();

protected:
	/// Splits 'Port' property for the blocks and returns motor devices on them. Implementation may consider
	/// that devices are non-null.
	template<class MotorType>
	QList<MotorType *> parsePorts(ReportErrors reportErrors = ReportErrors::report)
	{
		QList<MotorType *> result;
		QStringList const ports = eval<QStringList>("Ports", reportErrors);

		for (QString const &port : ports) {
			MotorType * const motor = robotModel::RobotModelUtils::findDevice<MotorType>(mRobotModel, port.trimmed());
			if (motor) {
				result << motor;
			}
		}

		return result;
	}

	/// @todo Move it to Block maybe?
	robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
