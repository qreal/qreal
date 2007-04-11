QT           += sql xml

HEADERS         = qsqlconnectiondialog.h   repo/realrepomodel.h   repo/realrepoitem.h
SOURCES         = qsqlconnectiondialog.cpp repo/realrepomodel.cpp repo/realrepoitem.cpp
FORMS           = qsqlconnectiondialog.ui

SOURCES       += tablemodel.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/sql/tablemodel
sources.files = $$SOURCES *.h $$RESOURCES $$FORMS tablemodel.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/sql/tablemodel
INSTALLS += target sources
