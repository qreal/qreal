#pragma once

#include <QtCore/QString>

#include <utils/timelineInterface.h>
#include <kitBase/robotModel/portInfo.h>

namespace twoDModel {
namespace engine {

class TwoDModelDisplayInterface;
class TwoDModelGuiFacade;

/// An interface for using 3D model in extensibility components.
/// Provides some basic engine methods like scanning the field or playing the sound or working with display.
class ThreeDModelEngineInterface
{
public:
	virtual ~ThreeDModelEngineInterface() {}

	// Block for 3D model
	/// Set new client ID of V-Rep for connection.
	virtual void setClientID(int newClientID) = 0;

	//

	/// Enables the motor on the given port assigning the given power to it with the given break mode.
	virtual void setNewMotor(int speed, uint degrees
			, const kitBase::robotModel::PortInfo &port, bool breakMode) = 0;

	/// Returns the encoder value on the given port.
	virtual int readEncoder(const kitBase::robotModel::PortInfo &port) const = 0;

	/// Nullifies the encoder value on the given port.
	virtual void resetEncoder(const kitBase::robotModel::PortInfo &port) = 0;

	/// Returns the collision sensor`s value on the given port.
	/// @returns 1 if collision is detected and 0 otherwise.
	virtual int readTouchSensor(const kitBase::robotModel::PortInfo &port) const = 0;

	/// Returns the distance value scanned by the range sensor.
	/// @returns The distance in cm till the closest object in the scan field of 255 if no such.
	virtual int readSonarSensor(const kitBase::robotModel::PortInfo &port) const = 0;

	/// Returns the configured color sensor`s value on the given port.
	/// It  may be the percent of some color under the sensor or the dominating color enum value.
	virtual int readColorSensor(const kitBase::robotModel::PortInfo &port) const = 0;

	/// Returns the average brightness of the picture under the light sensor on the given port.
	virtual int readLightSensor(const kitBase::robotModel::PortInfo &port) const = 0;

	/// Returns an image of the scene under a sensor on the given port.
	/// @param widthFactor ratio of a return area width to width of sensor image.
	virtual QImage areaUnderSensor(const kitBase::robotModel::PortInfo &port, qreal widthFactor) const = 0;

	/// Draws sound waves during the given amount of time thus showing that the robot plays some sound.
	virtual void playSound(int timeInMs) = 0;

	/// Return true if robot currently draws the trace on the floor behind it.
	virtual bool isMarkerDown() const = 0;

	/// Moves the marker of the 3D model robot down to the floor.
	/// The robot will draw its trace on the floor after that.
	/// If the marker of another color is already drawing at the moment it will be replaced.
	virtual void markerDown(const QColor &color) = 0;

	/// Lifts the marker of the 3D model robot up.
	/// The robot stops drawing its trace on the floor after that.
	virtual void markerUp() = 0;

	/// Retuns the timeline implementation for the 3D model time.
	virtual utils::TimelineInterface &modelTimeline() = 0;

	/// Returns a pointer to 3D model display emulator.
	virtual TwoDModelDisplayInterface *display() = 0;

	/// Returns an object for convenient searching and managing widgets of 3D model GUI.
	virtual engine::TwoDModelGuiFacade &guiFacade() const = 0;

};

}
}

