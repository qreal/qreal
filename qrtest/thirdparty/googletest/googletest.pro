# Copyright 2016 CyberTech Labs Ltd.
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

include(../../../global.pri)

TEMPLATE = lib

# CONFIG += warn_off

INCLUDEPATH += \
	$$PWD/googletest/ \
	$$PWD/googletest/include/ \
	$$PWD/googlemock/ \
	$$PWD/googlemock/include/ \

HEADERS += \
	$$files(googletest/include/gtest/*.h) \
	$$files(googletest/include/gtest/internal/*.h) \
	$$files(googlemock/include/gmock/*.h) \
	$$files(googlemock/include/gmock/internal/*.h) \

SOURCES += \
	$$PWD/googletest/src/gtest-all.cc \
	$$PWD/googlemock/src/gmock-all.cc \
