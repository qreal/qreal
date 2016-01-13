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

/*
 * LEGO® MINDSTORMS EV3
 *
 * Copyright (C) 2010-2013 The LEGO Group
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#pragma once

namespace ev3 {
namespace enums {

namespace commandType {
enum CommandTypeEnum {
	DIRECT_COMMAND_REPLY = 0x00 // Direct command, reply required
	, DIRECT_COMMAND_NO_REPLY = 0x80 // Direct command, reply not required
};
}


// The daisychain layer
namespace daisyChainLayer {
enum DaisyChainLayerEnum {
	EV3 = 0 // The EV3
	, FIRST = 1 // First EV3 in the Daisychain
	, SECOND = 2 // Second EV3 in the Daisychain
	, THIRD = 3 // Third EV3 in the Daisychain
};
}

namespace argumentSize {
enum ArgumentSizeEnum {
	BYTE = 0x81 // 1 byte
	, SHORT = 0x82 // 2 bytes
	, INT = 0x83 // 4 bytes
	, STRING = 0x84 // null-terminated string
};
}

namespace replyType {
enum ReplyTypeEnum {
	DIRECT_REPLY = 0x02 // Direct command reply
	, DIRECT_NO_REPLY = 0x04 // Direct command reply error
};
}

namespace opcode {
enum OpcodeEnum {
	UI_READ_GET_FIRMWARE = 0x810a

	, UI_WRITE_LED = 0x821b

	, KEEP_ALIVE = 0x90

	, UI_BUTTON_PRESSED = 0x8309

	, UI_DRAW_UPDATE = 0x8400
	, UI_DRAW_CLEAN = 0x8401
	, UI_DRAW_PIXEL = 0x8402
	, UI_DRAW_LINE = 0x8403
	, UI_DRAW_CIRCLE = 0x8404
	, UI_DRAW_TEXT = 0x8405
	, UI_DRAW_FILL_RECT = 0x8409
	, UI_DRAW_RECT = 0x840a
	, UI_DRAW_INVERSE_RECT = 0x8410
	, UI_DRAW_SELECT_FONT = 0x8411
	, UI_DRAW_TOP_LINE = 0x8412
	, UI_DRAW_FILL_WINDOW = 0x8413
	, UI_DRAW_DOT_LINE = 0x8415
	, UI_DRAW_FILL_CIRCLE = 0x8418
	, UI_DRAW_BMP_FILE = 0x841c

	, SOUND_BREAK = 0x9400
	, SOUND_TONE = 0x9401
	, SOUND_PLAY = 0x9402
	, SOUND_REPEAT = 0x9403
	, SOUND_SERVICE = 0x9404

	, INPUT_DEVICE_GET_TYPE_MODE = 0x9905
	, INPUT_DEVICE_GET_DEVICE_NAME = 0x9915
	, INPUT_DEVICE_GET_MODE_NAME = 0x9916
	, INPUT_DEVICE_READY_PCT = 0x991b
	, INPUT_DEVICE_READY_RAW = 0x991c
	, INPUT_DEVICE_READY_SI = 0x991d
	, INPUT_DEVICE_CLEAR_All = 0x990a
	, INPUT_DEVICE_CLEAR_CHANGES = 0x991a

	, INPUT_READ = 0x9a
	, INPUT_READ_EXT = 0x9e
	, INPUT_READ_SI = 0x9d
	, INPUT_WRITE = 0x9f

	, OUTPUT_STOP = 0xa3
	, OUTPUT_POWER = 0xa4
	, OUTPUT_SPEED = 0xa5
	, OUTPUT_START = 0xa6
	, OUTPUT_POLARITY = 0xa7
	, OUTPUT_STEP_POWER = 0xac
	, OUTPUT_TIME_POWER = 0xad
	, OUTPUT_STEP_SPEED = 0xae
	, OUTPUT_TIME_SPEED = 0xaf
	, OUTPUT_STEP_SYNC = 0xb0
	, OUTPUT_TIME_SYNC = 0xb1
	, OUTPUT_CLR_COUNT = 0xb2

	, TST = 0xff
};
}

// Format for sensor data.
namespace format {
enum FormatEnum {
	PERCENT = 0x10 // Percentage
	, RAW = 0x11 // Raw
	, SI = 0x12 // International System of Units
};
}

// Buttons on the face of the EV3 brick
namespace brickButton {
enum BrickButtonEnum {
	NONE // No button
	, UP // Up button
	, ENTER // Enter button
	, DOWN // Down button
	, RIGHT // Right button
	, LEFT // Left button
	, BACK // Back button
	, ANY // Any button
};
}

// UI colors
namespace color {
enum ColorEnum {
	BACKGROUND // Color of the background
	, FOREGROUND // Color of the foreground
};
}

namespace outputPort {
enum OutputPortEnum {
	A = 0x01
	, B = 0x02
	, C = 0x04
	, D = 0x08
	, ALL = 0x0f
};
}

namespace inputPort {
enum InputPortEnum {
	One = 0x00
	, Two = 0x01
	, Three = 0x02
	, Four = 0x03

	, A = 0x10
	, B = 0x11
	, C = 0x12
	, D = 0x13
};
}

namespace sensorType {
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
	, LOW_SPEED = 0x0A
	, LOW_SPEED_9V = 0x0B
	, SONAR_METRIC = 0x0C  // Not documented
	, COLOR_FULL = 0x0D
	, COLOR_RED = 0x0E
	, COLOR_GREEN = 0x0F
	, COLOR_BLUE = 0x10
	, COLOR_NONE = 0x11
};
}

}
}
