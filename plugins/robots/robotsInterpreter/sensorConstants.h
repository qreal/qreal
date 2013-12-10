#pragma once

#include <QtCore/QStringList>

namespace qReal {
namespace interpreters {
namespace robots {

namespace details {
namespace d2Model {
int const sensorWidth = 12;
}
}

namespace enums {

namespace sensorType {
enum SensorTypeEnum {
	unused
	, touchBoolean
	, touchRaw
	, sonar
	, light
	, colorFull
	, colorRed
	, colorGreen
	, colorBlue
	, colorNone
	, sound
	, gyroscope
	, accelerometer
	, encoder
};
}

namespace inputPort {
enum InputPortEnum {
	port1
	, port2
	, port3
	, port4
	, none
};
}

namespace robotModelType {
enum robotModelTypeEnum {
	null
	, twoD
	, nxt
	, trik
};
}
}

/// Contains everything about enumerating different NXT sensors types
class SensorEnumerator
{
public:
	/// Returns a list of all possible NXT sensors types
	static QStringList sensorNamesList();

	/// Converts internal sensor type to its friendly name
	static QString sensorName(enums::sensorType::SensorTypeEnum sensor);
};

}
}

}
