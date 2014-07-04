#pragma once

#include <QtCore/QString>

#include <utils/timelineInterface.h>
#include <interpreterBase/robotModel/portInfo.h>

namespace twoDModel {
namespace engine {

class TwoDModelDisplayInterface;

/// An interface for using 2D model in extensibility components.
/// Provides some basic engine methods like scanning the field or playing the sound or working with display.
class TwoDModelEngineInterface
{
public:
	virtual ~TwoDModelEngineInterface() {}

	/// Enables the motor on the given port assigning the given power to it with the given break mode.
	virtual void setNewMotor(int speed, uint degrees
			, interpreterBase::robotModel::PortInfo const &port, bool breakMode) = 0;

	/// Returns the encoder value on the given port.
	virtual int readEncoder(interpreterBase::robotModel::PortInfo const &port) const = 0;

	/// Nullifies the encoder value on the given port.
	virtual void resetEncoder(interpreterBase::robotModel::PortInfo const &port) = 0;

	/// Returns the collision sensor`s value on the given port.
	/// @returns 1 if collision is detected and 0 otherwise.
	virtual int readTouchSensor(interpreterBase::robotModel::PortInfo const &port) const = 0;

	/// Returns the distance value scanned by the range sensor.
	/// @returns The distance in cm till the closest object in the scan field of 255 if no such.
	virtual int readSonarSensor(interpreterBase::robotModel::PortInfo const &port) const = 0;

	/// Returns the configured color sensor`s value on the given port.
	/// It  may be the percent of some color under the sensor or the dominating color enum value.
	virtual int readColorSensor(interpreterBase::robotModel::PortInfo const &port) const = 0;

	/// Returns the average brightness of the picture under the light sensor on the given port.
	virtual int readLightSensor(interpreterBase::robotModel::PortInfo const &port) const = 0;

	/// Draws sound waves during the given amount of time thus showing that the robot plays some sound.
	virtual void playSound(int timeInMs) = 0;

	/// Retuns the timeline implementation for the 2D model time.
	virtual utils::TimelineInterface &modelTimeline() = 0;

	/// Returns a pointer to 2D model display emulator.
	virtual TwoDModelDisplayInterface *display() = 0;

	///Returns robot 2D model gui facade.
	virtual QObject *guiFacade();
};

}
}
