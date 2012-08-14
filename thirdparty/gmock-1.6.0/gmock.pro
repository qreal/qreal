TEMPLATE = lib
DESTDIR = ../../bin/thirdparty
TARGET = gmock

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	. \
	include \
	gtest \
	gtest/include \

HEADERS += \
	$$files(gtest/src/*.h) \
	$$files(gtest/include/gtest/*.h) \
	$$files(gtest/include/gtest/internal/*.h) \
	$$files(include/gmock/*.h) \
	$$files(include/gmock/internal/*.h) \

SOURCES += \
	$$files(gtest/src/*.cc) \
	$$files(src/*.cc) \
