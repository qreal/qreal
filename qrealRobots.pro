include (qreal.pri)

plugins.file = plugins/robots.pro

RESOURCES += \
    qrgui/mainwindow/qrgui.qrc

SUBDIRS += \
    qrgui/configurationNetworkManager/configurationNetworkManager \
    qrgui/configurationNetworkManager