/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

namespace nxt {

const unsigned maxMessageSize = 59;
const unsigned i2cBufferSize = 24;

namespace enums {

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
	, SONAR_METRIC = 0x0C  // Not documented
	, COLORFULL = 0x0D
	, COLORRED = 0x0E
	, COLORGREEN = 0x0F
	, COLORBLUE = 0x10
	, COLORNONE = 0x11
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

namespace sonarMode {
enum SonarModeEnum {
	OFF
	, SINGLE_SHOT
	, CONTINUOUS_MEASUREMENT
	, EVENT_CAPTURE
	, REQUEST_WARM_RESET
};
}

namespace sonarRegisters {
enum SonarRegistersEnum {
	SONAR_ADDRESS = 0x02
	, INTERVAL = 0x40
	, COMMAND = 0x41
	, RESULT_1 = 0x42
	, RESULT_2 = 0x43
	, RESULT_3 = 0x44
	, RESULT_4 = 0x45
	, RESULT_5 = 0x46
	, RESULT_6 = 0x47
	, RESULT_7 = 0x48
	, RESULT_8 = 0x49
	, ZERO_VALUE = 0x50
	, SCALE_FACTOR = 0x51
	, SCALE_DIVISOR = 0x52
	, FACTORY_ZERO_VALUE = 0x11
	, FACTORY_SCALE_FACTOR = 0x12
	, FACTORY_SCALE_DIVISOR = 0x13
};
}

namespace sonarReadingMode {
enum SonarReadingModeEnum {
	METRIC = 1
	, INCH = 2
};
}

}

}
