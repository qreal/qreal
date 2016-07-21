# Copyright 2013-2016 CyberTech Labs Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

QT += widgets

includes(plugins/robots/nxt/nxtCommon \
		plugins/robots/core/kitBase \
		plugins/robots/core/twoDModel \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base qextserialport qslog \
)

HEADERS += \
	$$PWD/include/nxtCommon/robotModel/nxtRobotModelBase.h \
	$$PWD/include/nxtCommon/robotModel/parts/nxtDisplay.h \
	$$PWD/include/nxtCommon/robotModel/parts/nxtSpeaker.h \
	$$PWD/include/nxtCommon/robotModel/parts/nxtMotor.h \
	$$PWD/include/nxtCommon/blocks/nxtBlocksFactory.h \
	$$PWD/src/blocks/details/speakerBlock.h \
	$$PWD/src/blocks/details/beepBlock.h \
	$$PWD/src/blocks/details/playToneBlock.h \
	$$PWD/src/blocks/details/drawCircleBlock.h \
	$$PWD/src/blocks/details/drawLineBlock.h \
	$$PWD/src/blocks/details/drawPixelBlock.h \
	$$PWD/src/blocks/details/drawRectBlock.h \
	$$PWD/src/blocks/details/nxtEnginesBackwardBlock.h \
	$$PWD/src/blocks/details/nxtEnginesForwardBlock.h \

SOURCES += \
	$$PWD/src/robotModel/nxtRobotModelBase.cpp \
	$$PWD/src/robotModel/parts/nxtDisplay.cpp \
	$$PWD/src/robotModel/parts/nxtSpeaker.cpp \
	$$PWD/src/robotModel/parts/nxtMotor.cpp \
	$$PWD/src/blocks/nxtBlocksFactory.cpp \
	$$PWD/src/blocks/details/speakerBlock.cpp \
	$$PWD/src/blocks/details/beepBlock.cpp \
	$$PWD/src/blocks/details/playToneBlock.cpp \
	$$PWD/src/blocks/details/drawCircleBlock.cpp \
	$$PWD/src/blocks/details/drawLineBlock.cpp \
	$$PWD/src/blocks/details/drawPixelBlock.cpp \
	$$PWD/src/blocks/details/drawRectBlock.cpp \
	$$PWD/src/blocks/details/nxtEnginesBackwardBlock.cpp \
	$$PWD/src/blocks/details/nxtEnginesForwardBlock.cpp \

TRANSLATIONS = \
	$$PWD/../../../../qrtranslations/ru/plugins/robots/nxtCommon_ru.ts \
	$$PWD/../../../../qrtranslations/fr/plugins/robots/nxtCommon_fr.ts \
