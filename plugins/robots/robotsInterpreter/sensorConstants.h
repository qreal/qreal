#pragma once

namespace qReal {
namespace interpreters {
namespace robots {

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

}
}
}
