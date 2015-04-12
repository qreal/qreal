TEMPLATE = lib

TARGET = qrgui-models

include(../../global.pri)

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_models_ru.ts

DEFINES += QRGUI_MODELS_LIBRARY

include(models.pri)
