# Copyright 2016 Dmitry Mordvinov
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

DEFINES += QRGRAPH_LIBRARY

includes(qrgraph)

HEADERS += \
	$$PWD/include/qrgraph/multigraph.h \
	$$PWD/include/qrgraph/node.h \
	$$PWD/include/qrgraph/edge.h \

SOURCES += \
	$$PWD/src/multigraph.cpp \
	$$PWD/src/node.cpp \
	$$PWD/src/edge.cpp \
