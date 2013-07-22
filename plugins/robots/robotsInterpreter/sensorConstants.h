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

class SensorEnumerator
{
public:

	static QStringList sensorNamesList();
	static QString sensorName(sensorType::SensorTypeEnum const &sensor);
};

}
}

}
