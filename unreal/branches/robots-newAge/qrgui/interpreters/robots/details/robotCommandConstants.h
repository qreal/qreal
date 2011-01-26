#pragma once

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

unsigned const maxMessageSize = 59;

namespace telegramType {
enum TelegramTypeEnum {
	directCommandResponseRequired = 0x00
	, systemCommandResponseRequired = 0x01
	, reply = 0x02
	, directCommandNoResponse = 0x80
	, systemCommandNoResponse = 0x81
};
}

namespace commandCode {
enum CommandCodeEnum {
	STARTPROGRAM
	, STOPPROGRAM
	, PLAYSOUNDFILE
	, PLAYTONE
	, SETOUTPUTSTATE  // = 0x04
	, SETINPUTMODE
	, GETOUTPUTSTATE
	, GETINPUTVALUES
	, RESETINPUTSCALEDVALUE  // 0x08
	, MESSAGEWRITE
	, RESETMOTORPOSITION
	, GETBATTERYLEVEL
	, STOPSOUNDPLAYBACK  // = 0x0C
	, KEEPALIVE
	, LSGETSTATUS
	, LSWRITE
	, LSREAD  // = 0x10
	, GETCURRENTPROGRAMNAME
	, MESSAGEREAD
};
}

namespace errorCode {
enum ErrorCodeEnum {
	success = 0x00
	, pendingCommunicationTransactionInProgress = 0x20
	, specifiedMailboxQueueIsEmpty = 0x40
	, requestFailed = 0xBD
	, unknownCommandOpcode = 0xBE
	, insanePacket = 0xBF
	, dataContainsOutOfRangeValues = 0xC0
	, communicationBusError = 0xDD
	, noFreeMemoryInCOmmunicationBuffer = 0xDE
	, specifiedChannelOrConnectionIsNotValid = 0xDF
	, specifiedChannelOrConnectionNotConfiguredOrBusy = 0xE0
	, noActiveProgram = 0xEC
	, illegalSizeSpecified = 0xED
	, illegalMailboxQueueIdSpecified = 0xEE
	, attemptedToAccessInvalidFieldOfAStructure = 0xEF
	, badInputOrOutputSpecified = 0xF0
	, insufficientMemoryAvailable = 0xFB
	, badArguments = 0xFF
};
}

namespace outputPort {
enum OutputPortEnum {
	port1
	, port2
	, port3
	, all = 0xFF
};
}

namespace motorMode {
enum MotorModeEnum {
	MOTORON = 0x01
	, BRAKE = 0x02
	, REGULATED = 0x04
};
}

namespace regulationMode {
enum RegulationModeEnum {
	REGULATION_MODE_IDLE = 0x00
	, REGULATION_MODE_MOTOR_SPEED = 0x01
	, REGULATION_MODE_MOTOR_SYNC = 0x02
};
}

namespace runState {
enum RunStateEnum {
	MOTOR_RUN_STATE_IDLE = 0x00
	, MOTOR_RUN_STATE_RAMPUP = 0x10
	, MOTOR_RUN_STATE_RUNNING = 0x20
	, MOTOR_RUN_STATE_RAMPDOWN = 0x40
};
}

namespace lowLevelInputPort {
enum InputPortEnum {
	port1
	, port2
	, port3
	, port4
};
}

namespace lowLevelSensorType {
enum SensorTypeEnum {
	NO_SENSOR = 0x00
	, SWITCH = 0x01
	, TEMPERATURE = 0x02
	, REFLECTION = 0x03
	, ANGLE = 0x04
	, LIGHT_ACTIVE = 0x05
	, LIGHT_INACTIVE = 0x06
	, SOUND_DB = 0x07
	, SOUND_DBA = 0x08
	, CUSTOM = 0x09
	, LOWSPEED = 0x0A
	, LOWSPEED_9V = 0x0B

	// Some not documented sensors, from Bluetooth Communication Library sources
	, SONAR_METRIC = 0x0C
	, SONAR_INCH = 0x0D
	, COMPASS = 0x0E
	, IO_8574_SENSOR = 0x0F
	, ADC_8591_SENSOR = 0x10
	, COLOR = 0x11
	, GYRO = 0x12
	, TILT = 0x13
};
}

namespace sensorMode {
enum SensorModeEnum {
	RAWMODE = 0x00
	, BOOLEANMODE = 0x20
	, TRANSITIONCNTMODE = 0x40
	, PERIODCOUNTERMODE = 0x60
	, PCTFULLSCALEMODE = 0x80
	, CELSIUSMODE = 0xA0
	, FAHRENHEITMODE = 0xC0
	, ANGLESTEPSMODE = 0xE0
	, SLOPEMASK = 0x1F
	, MODEMASK = 0xE0
};
}

}
}
}
}
