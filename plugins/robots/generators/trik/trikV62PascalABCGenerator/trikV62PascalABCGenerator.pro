TARGET = robots-trik-v62-pascal-abc-generator

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes( \
                plugins/robots/generators/trik/trikPascalABCGeneratorLibrary \
                plugins/robots/generators/trik/trikGeneratorBase \
                plugins/robots/generators/generatorBase \
                plugins/robots/common/kitBase \
                plugins/robots/common/trikKit \
                plugins/robots/utils \
                qrtext \
)

links(robots-trik-pascal-abc-generator-library robots-trik-generator-base robots-generator-base robots-trik-kit \
                robots-kit-base \
)

HEADERS += \
        $$PWD/trikV62PascalABCGeneratorPlugin.h

SOURCES += \
        $$PWD/trikV62PascalABCGeneratorPlugin.cpp
