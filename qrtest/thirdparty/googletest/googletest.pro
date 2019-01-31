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

CONFIG += noPch

include(../../../global.pri)

TEMPLATE = lib

# CONFIG += warn_off

#DEFINES += GTEST_HAS_STREAM_REDIRECTION=1 GTEST_LANG_CXX11=1 GTEST_HAS_STD_WSTRING=1 \
#           GTEST_HAS_RTTI=1 \
#           GTEST_HAS_PTHREAD=0 \
#           GTEST_HAS_EXCEPTIONS=1 \
#           GTEST_LINKED_AS_SHARED_LIBRARY=1 \
#           GTEST_CREATE_SHARED_LIBRARY=1

INCLUDEPATH += \
	$$PWD/googletest/googletest/ \
	$$PWD/googletest/googletest/include/ \
	$$PWD/googletest/googlemock/ \
	$$PWD/googletest/googlemock/include/ \

HEADERS += \
	$$files(googletest/googletest/include/gtest/*.h) \
	$$files(googletest/googletest/include/gtest/internal/*.h) \
	$$files(googletest/googlemock/include/gmock/*.h) \
	$$files(googletest/googlemock/include/gmock/internal/*.h) \

SOURCES += \
	$$PWD/googletest/googletest/src/gtest-all.cc \
	$$PWD/googletest/googlemock/src/gmock-all.cc \
