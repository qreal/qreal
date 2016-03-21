TARGET = robots-trik-pascal-abc-generator-library

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib

includes(plugins/robots/generators/trik/trikGeneratorBase \
                plugins/robots/generators/generatorBase \
                plugins/robots/common/kitBase \
                plugins/robots/common/trikKit \
                plugins/robots/utils \
                qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base \
                robots-kit-base robots-trik-kit robots-utils \
)

HEADERS += \
        $$PWD/include/trikPascalABCGeneratorLibrary/trikPascalABCGeneratorPluginBase.h \
        $$PWD/src/trikPascalABCMasterGenerator.h \
        $$PWD/src/trikPascalABCControlFlowValidator.h

SOURCES += \
        $$PWD/src/trikPascalABCGeneratorPluginBase.cpp \
        $$PWD/src/trikPascalABCMasterGenerator.cpp \
        $$PWD/src/trikPascalABCControlFlowValidator.cpp

RESOURCES += \
        $$PWD/trikPascalABCGeneratorLibrary.qrc \
        $$PWD/templates.qrc

