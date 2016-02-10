# Copyright 2007-2016 QReal Research Group, CyberTech Labs Ltd.
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

TARGET = test-utils

include($$PWD/../common.pri)

DEFINES += TEST_UTILS_LIBRARY

TEMPLATE = lib

HEADERS += \
	$$PWD/include/testUtils/declSpec.h \
	$$PWD/include/testUtils/delay.h \
	$$PWD/include/testUtils/fakeSender.h \
	$$PWD/include/testUtils/qrguiFacade.h \
	$$PWD/include/testUtils/signalsTester.h \

SOURCES += \
	$$PWD/src/delay.cpp \
	$$PWD/src/qrguiFacade.cpp \
	$$PWD/src/signalsTester.cpp \

links(qrgui-tool-plugin-interface qrgui-models)

#links(qrkernel qrrepo qrutils qrtext qrgui-models qrgui-controller qrgui-preferences-dialog qrgui-plugin-manager \
#		qrgui-tool-plugin-interface robots-kit-base robots-utils robots-2d-model qslog \
#)
