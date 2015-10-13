# Copyright 2007-2015 QReal Research Group
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

DEFINES += QRKERNEL_LIBRARY
HEADERS += \
	$$PWD/ids.h \
	$$PWD/definitions.h \
	$$PWD/exception/exception.h \
	$$PWD/roles.h \
	$$PWD/settingsManager.h \
	$$PWD/settingsListener.h \
	$$PWD/kernelDeclSpec.h \
	$$PWD/timeMeasurer.h \
	$$PWD/version.h \
	$$PWD/logging.h \
	$$PWD/platformInfo.h \
	$$PWD/private/listeners.h \

SOURCES += \
	$$PWD/ids.cpp \
	$$PWD/exception/exception.cpp \
	$$PWD/settingsManager.cpp \
	$$PWD/settingsListener.cpp \
	$$PWD/timeMeasurer.cpp \
	$$PWD/version.cpp \
	$$PWD/logging.cpp \
	$$PWD/platformInfo.cpp \

RESOURCES += \
	$$PWD/qrkernel.qrc \

includes(thirdparty/qslog)
links(qslog)
