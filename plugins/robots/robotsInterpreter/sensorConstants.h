#pragma once

#include <QStringList>

namespace qReal {
namespace interpreters {
namespace robots {

int const sensorWidth = 10;

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
	, real
	, unreal
};
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
