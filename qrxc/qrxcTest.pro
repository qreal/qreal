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

TEMPLATE = app
QT += xml gui
CONFIG += console

macx {
	CONFIG -= app_bundle
}

MOC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L../bin -lqrutils

DESTDIR += ../bin

!macx {
	QMAKE_LFLAGS+="-Wl,-O1,-rpath,$$PWD/../bin"
}

INCLUDEPATH += \
	../thirdparty/gmock-1.6.0/include \
	../thirdparty/gmock-1.6.0/gtest/include \

LIBS += -L../bin/thirdparty/ -lgmock -lpthread

TARGET = qrxc_unittests
DESTDIR = ../bin/unittests
DEFINES += UNITTEST

HEADERS += association.h \
	diagram.h \
	edgeType.h \
	editor.h \
	enumType.h \
	graphicType.h \
	label.h \
	linePort.h \
	nameNormalizer.h \
	nodeType.h \
	nonGraphicType.h \
	numericType.h \
	pointPort.h \
	port.h \
	portType.h \
	property.h \
	sdftocpp.h \
	stringType.h \
	type.h \
	xmlCompiler.h \

SOURCES += association.cpp \
	diagram.cpp \
	edgeType.cpp \
	editor.cpp \
	enumType.cpp \
	graphicType.cpp \
	label.cpp \
	linePort.cpp \
	mainTest.cpp \
	nameNormalizer.cpp \
	nodeType.cpp \
	nonGraphicType.cpp \
	numericType.cpp \
	pointPort.cpp \
	port.cpp \
	portType.cpp \
	property.cpp \
	sdftocpp.cpp \
	stringType.cpp \
	type.cpp \
	xmlCompiler.cpp \

# Unit tests
include (unitTests/unitTests.pri)
