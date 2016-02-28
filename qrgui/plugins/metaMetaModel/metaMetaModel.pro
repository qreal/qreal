# Copyright 2007-2016 QReal Research Group
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

TARGET = qrgui-meta-meta-model

include(../../../global.pri)

TEMPLATE = lib

includes(qrgui qrgraph)

links(qrkernel qrgraph)

HEADERS += \
	$$PWD/include/metaMetaModel/metamodel.h \
	$$PWD/include/metaMetaModel/elementType.h \
	$$PWD/include/metaMetaModel/nodeElementType.h \
	$$PWD/include/metaMetaModel/edgeElementType.h \
	$$PWD/include/metaMetaModel/linkShape.h \
	$$PWD/include/metaMetaModel/elementRepoInterface.h \
	$$PWD/include/metaMetaModel/sdfRendererInterface.h \
	$$PWD/include/metaMetaModel/labelProperties.h \
	$$PWD/include/metaMetaModel/portHelpers.h \

SOURCES += \
	$$PWD/src/metamodel.cpp \
	$$PWD/src/elementType.cpp \
	$$PWD/src/nodeElementType.cpp \
	$$PWD/src/edgeElementType.cpp \
	$$PWD/src/labelProperties.cpp \
